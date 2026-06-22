// SPDX-License-Identifier: BSD-3-Clause

#include "VideoEncoder.hpp"

#include <array>
#include <cstring>
#include <stdexcept>
#include <string_view>

extern "C"
{
#include <libavutil/error.h>
}

#include "util/format.hpp"

#if __has_include(<ffnvcodec/nvEncodeAPI.h>)
extern "C"
{
    #include <ffnvcodec/nvEncodeAPI.h>
}
#else
    #define NVENC_INFINITE_GOPLENGTH 0xffffffff
#endif

namespace
{

const char* avErr(int errnum)
{
    static thread_local std::array<char, AV_ERROR_MAX_STRING_SIZE> errstr;
    av_make_error_string(&errstr[0], errstr.size(), errnum);
    return &errstr[0];
}

// Precomputed lookup table for the grayscale -> Y mapping Y = 0.895 * W + 16. Replaces a
// per-pixel floating-point multiply over millions of pixels with a single table lookup, removing
// the conversion as a consumer bottleneck. The values are bit-identical to the original formula
// (truncating cast, range [16, 244]).
struct GrayscaleToYTable final
{
    std::uint8_t value[256];

    GrayscaleToYTable()
    {
        for (int i = 0; i < 256; i++)
        {
            value[i] = (std::uint8_t)(0.895 * i + 16);
        }
    }
};
static const GrayscaleToYTable grayscaleToYTable;

void grayscaleToYUV420_y(const std::uint8_t* grayscale,
                         int                 width,
                         int                 height,
                         std::uint8_t*       yChannel)
{
    /*
     * Grayscale -> YUV:  Y[i] = 0.895 * W[i] + 16.   (U and V are constant, see below.)
     */
    const int count = width * height;
    for (int i = 0; i < count; i++)
    {
        yChannel[i] = grayscaleToYTable.value[grayscale[i]];
    }
}

void grayscaleToYUV420_uv(int width, int height, std::uint8_t* uChannel, std::uint8_t* vChannel)
{
    /*
     * Grayscale -> YUV:  U[i] = 128 (fixed),  V[i] = 128 (fixed).
     */
    std::memset(uChannel, 128, width * height / 4);
    std::memset(vChannel, 128, width * height / 4);
}

bool isNvenc(std::string_view codecName)
{
    return codecName == "hevc_nvenc" || codecName == "h264_nvenc";
}

} // namespace

VideoEncoder::VideoEncoder(Config config)
: _cfg(config)
, _codecContext(nullptr)
, _videoFrame(nullptr)
, _videoPacket(nullptr)
, _pts(0)
{
    const auto* codec = avcodec_find_encoder_by_name(_cfg.codec.name.c_str());
    if (!codec)
    {
        throw std::runtime_error(fmt::format("Could not find video encoder: {}", _cfg.codec.name));
    }

    const auto codecName = std::string_view(codec->name);
    if (codecName == "nvenc_hevc")
    {
        throw std::runtime_error(
            "Legacy nvenc_hevc encoder is not supported, use newer hevc_nvenc instead");
    }

    if (!(_codecContext = avcodec_alloc_context3(codec)))
    {
        throw std::runtime_error("Could not allocate video encoder state");
    }

    _codecContext->width     = _cfg.width;
    _codecContext->height    = _cfg.height;
    _codecContext->framerate = {_cfg.framerate.num, _cfg.framerate.den};
    _codecContext->time_base = {_cfg.framerate.den, _cfg.framerate.num};
    _codecContext->pix_fmt   = AV_PIX_FMT_YUV420P;

    // Output is an MP4/MOV container, which stores codec parameter sets (extradata) in the
    // container header. The encoder must therefore emit a global header rather than in-band
    // parameter sets so that each per-file stream gets a valid hvcc/avcc box.
    _codecContext->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

    if (isNvenc(codecName))
    {
        _codecContext->bit_rate = 0;

        // The encoder session is reused across many output files. For each file to be a
        // standalone, decodable clip we must be able to (a) force an IDR at the file boundary
        // and (b) have NO frames buffered inside the encoder when we close a file. Configure
        // zero output latency so that every avcodec_send_frame() yields exactly one packet
        // immediately and nothing spills across the boundary.
        av_opt_set(_codecContext->priv_data, "bf", "0", 0);           // no B-frames
        av_opt_set(_codecContext->priv_data, "rc-lookahead", "0", 0); // no look-ahead
        av_opt_set(_codecContext->priv_data, "delay", "0", 0);        // emit packets immediately
    }

    for (const auto& [name, value] : _cfg.codec.options)
    {
        if (name == "goplength" && isNvenc(codecName))
        {
            if (value == "infinite")
            {
                _codecContext->gop_size = NVENC_INFINITE_GOPLENGTH;
            }
        }
        else
        {
            if (auto r = av_opt_set(_codecContext->priv_data, name.c_str(), value.c_str(), 0);
                r < 0)
            {
                throw std::runtime_error(fmt::format(
                    "Failed to set encoder option {} to {}: {}", name, value, avErr(r)));
            }
        }
    }

    if (isNvenc(codecName))
    {
        // Keep forced-IDR enabled regardless of user options: it is what lets us start every
        // output file with an IDR keyframe (see encode()), even with an (effectively) infinite
        // GOP shared across files.
        av_opt_set(_codecContext->priv_data, "forced-idr", "1", 0);
    }

    if (auto r = avcodec_open2(_codecContext, codec, NULL); r < 0)
    {
        throw std::runtime_error(fmt::format("Could not initialize video encoder: {}", avErr(r)));
    }

    if (!(_videoFrame = av_frame_alloc()))
    {
        throw std::runtime_error("Could not allocate video frame");
    }

    _videoFrame->format = _codecContext->pix_fmt;
    _videoFrame->width  = _codecContext->width;
    _videoFrame->height = _codecContext->height;

    if (auto r = av_frame_get_buffer(_videoFrame, 0); r < 0)
    {
        throw std::runtime_error(fmt::format("Could not allocate video frame data: {}", avErr(r)));
    }

    // As we reuse the same frame, the constant U and V channels only need to be set once.
    grayscaleToYUV420_uv(_videoFrame->width,
                         _videoFrame->height,
                         _videoFrame->data[1],
                         _videoFrame->data[2]);

    if (!(_videoPacket = av_packet_alloc()))
    {
        throw std::runtime_error("Could not allocate video packet");
    }
}

VideoEncoder::~VideoEncoder()
{
    if (_videoPacket)
    {
        av_packet_free(&_videoPacket);
    }

    if (_videoFrame)
    {
        av_frame_free(&_videoFrame);
    }

    if (_codecContext)
    {
        avcodec_free_context(&_codecContext);
    }
}

void VideoEncoder::sendFrame(const GrayscaleImage& image, bool forceKeyframe)
{
    if (auto r = av_frame_make_writable(_videoFrame); r < 0)
    {
        throw std::runtime_error(fmt::format("Could not make video frame writable: {}", avErr(r)));
    }

    if ((image.width != _videoFrame->width) || (image.height != _videoFrame->height))
    {
        throw std::runtime_error(
            fmt::format("Could not write image to video: Got resolution {}x{} instead of {}x{}",
                        image.width,
                        image.height,
                        _videoFrame->width,
                        _videoFrame->height));
    }

    grayscaleToYUV420_y(&image.data[0], image.width, image.height, _videoFrame->data[0]);

    // Continuous, monotonically increasing pts for the whole session -- never reset, or the
    // reused encoder sees a backwards timestamp jump and produces non-monotonic DTS.
    _videoFrame->pts = _pts++;
    // Force an IDR keyframe for the first frame of a new file (forced-idr is enabled above),
    // otherwise let the encoder choose (P-frame). pict_type must be reset every frame as the
    // frame object is reused.
    _videoFrame->pict_type = forceKeyframe ? AV_PICTURE_TYPE_I : AV_PICTURE_TYPE_NONE;

    if (auto r = avcodec_send_frame(_codecContext, _videoFrame); r < 0)
    {
        throw std::runtime_error(fmt::format("Could not send frame to encoder: {}", avErr(r)));
    }
}

AVPacket* VideoEncoder::receivePacket()
{
    auto r = avcodec_receive_packet(_codecContext, _videoPacket);
    if (r == AVERROR(EAGAIN) || r == AVERROR_EOF)
    {
        return nullptr;
    }
    else if (r < 0)
    {
        throw std::runtime_error(fmt::format("Could not encode frame: {}", avErr(r)));
    }

    return _videoPacket;
}

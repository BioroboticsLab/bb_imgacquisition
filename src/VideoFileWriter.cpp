// SPDX-License-Identifier: BSD-3-Clause

#include "VideoFileWriter.hpp"

#include <array>
#include <stdexcept>

extern "C"
{
#include <libavutil/error.h>
}

#include "util/format.hpp"

struct FFmpegInitializer final
{
    FFmpegInitializer()
    {
#if LIBAVFORMAT_VERSION_MAJOR < 58
        av_register_all();
#endif
        av_log_set_level(AV_LOG_WARNING);
    }
};
static FFmpegInitializer initFFmpeg;

static const char* av_strerror(int errnum)
{
    static thread_local std::array<char, AV_ERROR_MAX_STRING_SIZE> errstr;
    av_make_error_string(&errstr[0], errstr.size(), errnum);
    return &errstr[0];
}

VideoFileWriter::VideoFileWriter(const std::string& filename, VideoEncoder& encoder)
: _encoder(encoder)
, _filename(filename)
, _formatContext(nullptr)
, _stream(nullptr)
, _dtsOffset(AV_NOPTS_VALUE)
, _firstFrame(true)
{
    // Set up a single output file for the shared encoder, with the container format automatically
    // detected based on the filename suffix. The encoder (codec) state is owned by `encoder` and
    // is reused across files; only the muxer below is per-file.

    AVCodecContext* codecContext = _encoder.codecContext();
    const AVCodec*  codec        = codecContext->codec;

    if (auto r = avformat_alloc_output_context2(&_formatContext, NULL, NULL, _filename.c_str());
        r < 0)
    {
        throw std::runtime_error(fmt::format("{}: Could not allocate video container state: {}",
                                             _filename,
                                             av_strerror(r)));
    }

    if (!avformat_query_codec(_formatContext->oformat, codec->id, FF_COMPLIANCE_VERY_STRICT))
    {
        throw std::runtime_error(
            fmt::format("{}: Cannot store {} encoded video in an {} container",
                        _filename,
                        codec->name,
                        _formatContext->oformat->name));
    }

    if (auto r = avio_open2(&_formatContext->pb, _filename.c_str(), AVIO_FLAG_WRITE, NULL, NULL);
        r < 0)
    {
        throw std::runtime_error(
            fmt::format("{}: Could not open for writing: {}", _filename, av_strerror(r)));
    }

    if (!(_stream = avformat_new_stream(_formatContext, codec)))
    {
        throw std::runtime_error(fmt::format("{}: Could not create new video stream", _filename));
    }

    if (auto r = avcodec_parameters_from_context(_stream->codecpar, codecContext); r < 0)
    {
        throw std::runtime_error(
            fmt::format("{}: Could not copy video encoder settings to video stream: {}",
                        _filename,
                        av_strerror(r)));
    }

    _stream->time_base      = codecContext->time_base;
    _stream->avg_frame_rate = codecContext->framerate;
    _stream->r_frame_rate   = codecContext->framerate;

    if (auto r = avformat_write_header(_formatContext, NULL); r < 0)
    {
        throw std::runtime_error(fmt::format("{}: Could not write video container header: {}",
                                             _filename,
                                             av_strerror(r)));
    }
}

VideoFileWriter::~VideoFileWriter()
{
    // Only the per-file muxer is released here; the shared encoder session is left intact.
    if (_formatContext)
    {
        if (_formatContext->pb)
        {
            avio_closep(&_formatContext->pb);
        }
        avformat_free_context(_formatContext);
        _formatContext = nullptr;
    }
}

void VideoFileWriter::write(const GrayscaleImage& image)
{
    // The first frame of every file is forced to be an IDR keyframe so the file is standalone.
    _encoder.sendFrame(image, _firstFrame);
    _firstFrame = false;

    while (AVPacket* packet = _encoder.receivePacket())
    {
        // The encoder timestamps are continuous across the whole run; shift this file's packets so
        // that the first packet of the file starts at zero (keeps the muxer's DTS monotonic and
        // each file independently timed).
        if (_dtsOffset == AV_NOPTS_VALUE)
        {
            _dtsOffset = (packet->dts != AV_NOPTS_VALUE) ? packet->dts : packet->pts;
        }

        if (packet->pts != AV_NOPTS_VALUE)
        {
            packet->pts -= _dtsOffset;
        }
        if (packet->dts != AV_NOPTS_VALUE)
        {
            packet->dts -= _dtsOffset;
        }

        av_packet_rescale_ts(packet, _encoder.timeBase(), _stream->time_base);
        packet->stream_index = _stream->index;

        av_write_frame(_formatContext, packet);
        av_packet_unref(packet);
    }
}

void VideoFileWriter::close()
{
    // With the zero-latency encoder configuration there are no frames buffered inside the encoder
    // at this point, so we only need to finalize the container. The encoder is NOT flushed or
    // freed here -- it stays alive for the next file.
    if (auto r = av_write_trailer(_formatContext); r < 0)
    {
        throw std::runtime_error(fmt::format("{}: Could not write video container trailer: {}",
                                             _filename,
                                             av_strerror(r)));
    }
}

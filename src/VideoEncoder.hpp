// SPDX-License-Identifier: BSD-3-Clause
#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/frame.h>
#include <libavutil/opt.h>
}

#include "GrayscaleImage.hpp"

/**
 * @brief A persistent (long-lived) video encoder wrapping a single libavcodec/nvenc session.
 *
 * The encoder session (AVCodecContext) is expensive to create and, for hardware encoders such
 * as nvenc, creating and destroying one per output file leaks resources in the driver and
 * spawns worker threads that are never reclaimed. This class therefore owns ONE session for the
 * whole lifetime of a recording and is reused across many output files; the per-file container
 * (muxer) is handled separately by VideoFileWriter.
 *
 * Because the session is shared across files, the input frames are given a SINGLE continuous,
 * monotonically increasing presentation timestamp for the whole run (resetting it per file would
 * feed the encoder a backwards timestamp jump and corrupt its DTS bookkeeping). The per-file
 * muxer is responsible for rebasing each file's packet timestamps back to zero.
 *
 * To make each output file independently decodable the encoder is configured (for nvenc) for
 * zero output latency -- no B-frames, no look-ahead, no delay -- so that every submitted frame
 * yields exactly one packet immediately and no frames remain buffered inside the encoder at a
 * file boundary. An IDR keyframe is forced at the first frame of each new file (see
 * sendFrame(..., forceKeyframe)).
 */
class VideoEncoder final
{
public:
    struct Config final
    {
        int width;
        int height;

        struct
        {
            int num;
            int den;
        } framerate;

        struct
        {
            std::string                                  name;
            std::unordered_map<std::string, std::string> options;
        } codec;
    };

    explicit VideoEncoder(Config config);
    ~VideoEncoder();

    VideoEncoder(const VideoEncoder&) = delete;
    VideoEncoder& operator=(const VideoEncoder&) = delete;
    VideoEncoder(VideoEncoder&&)                 = delete;
    VideoEncoder& operator=(VideoEncoder&&) = delete;

    const Config& config() const
    {
        return _cfg;
    }

    AVRational timeBase() const
    {
        return _codecContext->time_base;
    }

    AVCodecContext* codecContext() const
    {
        return _codecContext;
    }

    /**
     * Submit one image to the encoder. The presentation timestamp is assigned automatically from
     * a continuous, monotonically increasing counter that is NEVER reset (required for a session
     * reused across files). When @p forceKeyframe is true the frame is encoded as an IDR keyframe;
     * this must be done for the first frame of every new output file so that the file is
     * standalone-decodable.
     */
    void sendFrame(const GrayscaleImage& image, bool forceKeyframe);

    /**
     * Retrieve the next encoded packet, or nullptr if none is available right now. The returned
     * packet is owned by the encoder and remains valid until the next call; the caller must
     * av_packet_unref() it once it has been written. Packet timestamps are in timeBase().
     */
    AVPacket* receivePacket();

private:
    Config _cfg;

    AVCodecContext* _codecContext;
    AVFrame*        _videoFrame;
    AVPacket*       _videoPacket;

    // Continuous presentation timestamp, shared across all output files. Never reset.
    std::int64_t _pts;
};

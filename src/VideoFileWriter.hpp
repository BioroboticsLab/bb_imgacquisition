// SPDX-License-Identifier: BSD-3-Clause
#pragma once

#include <cstdint>
#include <string>

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

#include "GrayscaleImage.hpp"
#include "VideoEncoder.hpp"

/**
 * @brief Writes a single output video file (container/muxer) using a shared, persistent
 * VideoEncoder.
 *
 * One VideoFileWriter is created per output file. It owns only the per-file container state
 * (AVFormatContext, the output stream and the I/O handle) and BORROWS the long-lived encoder
 * session, which is deliberately NOT torn down when the file closes. This avoids creating and
 * destroying a hardware encoder session for every file -- which leaked driver memory and worker
 * threads and caused unbounded memory growth during long recordings.
 */
class VideoFileWriter final
{
public:
    VideoFileWriter(const std::string& filename, VideoEncoder& encoder);
    ~VideoFileWriter();

    VideoFileWriter(const VideoFileWriter&) = delete;
    VideoFileWriter& operator=(const VideoFileWriter&) = delete;

    void write(const GrayscaleImage& image);
    void close();

private:
    VideoEncoder& _encoder;

    std::string _filename;

    AVFormatContext* _formatContext;
    AVStream*        _stream;

    // The encoder uses one continuous timestamp for the whole run; rebase this file's packets so
    // each output file starts at zero. Captured from the first packet written to this file.
    std::int64_t _dtsOffset;
    bool         _firstFrame;
};

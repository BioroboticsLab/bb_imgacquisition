// SPDX-License-Identifier: BSD-3-Clause

#include <chrono>
#include <fstream>
#include <memory>
#include <optional>
#include <unordered_map>

#include <boost/filesystem.hpp>

#include "Settings.hpp"
// NOTE: The order of the includes below is important to avoid name clashes!
#include "ImageStreamsWriter.hpp"

#include "VideoEncoder.hpp"
#include "VideoFileWriter.hpp"

#include "util/format.hpp"
#include "util/log.hpp"

ImageStreamsWriter::ImageStreamsWriter(std::string encoderName)
: _encoderName{encoderName}
{
}

void ImageStreamsWriter::add(ImageStream imageStream)
{
    _imageStreams.push_back(std::move(imageStream));
}

void ImageStreamsWriter::run()
{
    const auto& settings = Settings::instance();

    // Persistent video encoders, one per image stream id. The (hardware) encoder session is
    // created once and reused across all output files of a stream instead of being created and
    // destroyed per file. Re-creating a hardware encoder per file leaked driver memory and worker
    // threads and was the cause of unbounded memory growth during long recordings.
    std::unordered_map<std::string, std::unique_ptr<VideoEncoder>> encoders;

    while (!isInterruptionRequested())
    {
        std::vector<std::size_t> sizes;
        for (auto& s : _imageStreams)
        {
            const auto [width, height] = s.resolution;
            sizes.emplace_back(s.size() * width * height);
        }

        const auto maxSize      = std::max_element(sizes.begin(), sizes.end());
        const auto maxSizeIndex = std::distance(sizes.begin(), maxSize);

        // If all are empty, check again later.
        if (maxSize == sizes.end() || *maxSize == 0)
        {
            usleep(500);
            continue;
        }

        auto imageStream           = _imageStreams[maxSizeIndex];
        const auto [width, height] = imageStream.resolution;

        // Surface a growing backlog (diagnostic only -- frames are never dropped; the queue
        // buffers them). With a fast enough consumer the queue stays near empty and any temporary
        // backlog drains. A persistently growing backlog means the consumer (encoder/disk) cannot
        // keep up and memory will grow -- the signal to investigate consumer throughput.
        const auto backlog = imageStream.size();
        if (backlog > 2 * imageStream.framesPerFile)
        {
            logWarning("{}: encoder backlog {} frames (~{} MiB) buffered, consumer falling behind",
                       imageStream.id,
                       backlog,
                       (backlog * width * height) / (1024 * 1024));
        }
        else
        {
            logDebug("{}: queue depth {} frames", imageStream.id, backlog);
        }

        // Get (or lazily create) the persistent encoder for this stream.
        VideoEncoder* encoder = nullptr;
        try
        {
            auto it = encoders.find(imageStream.id);
            if (it == encoders.end())
            {
                it = encoders
                         .emplace(imageStream.id,
                                  std::make_unique<VideoEncoder>(VideoEncoder::Config{
                                      static_cast<int>(width),
                                      static_cast<int>(height),
                                      {static_cast<int>(imageStream.framesPerSecond), 1},
                                      {_encoderName, imageStream.encoderOptions}}))
                         .first;
                logInfo("{}: Created persistent video encoder [reuse+rebase+boundedq build]",
                        imageStream.id);
            }
            encoder = it->second.get();
        }
        catch (const std::exception& e)
        {
            logCritical("{}: Failed to create video encoder: {}", imageStream.id, e.what());
            usleep(500000);
            continue;
        }

        const auto startProcessingTime = std::chrono::system_clock::now();

        namespace fs = boost::filesystem;

        const auto tmpDir = fs::path{settings.temporaryDirectory()} / imageStream.id;
        if (!fs::exists(tmpDir))
        {
            fs::create_directories(tmpDir);
        }

        const auto tmpVideoFilename = tmpDir / fmt::format("{}.mp4", startProcessingTime);

        const auto tmpFrameTimestampsFilename = tmpDir /
                                                fmt::format("{}.txt", startProcessingTime);

        std::fstream frameTimestamps(tmpFrameTimestampsFilename.string(),
                                     std::ios::trunc | std::ios::out);

        const std::size_t debugInterval = 100;

        auto startFrameTime = std::optional<std::chrono::system_clock::time_point>{};
        auto endFrameTime   = std::optional<std::chrono::system_clock::time_point>{};

        bool        imageStreamClosedEarly = false;
        bool        encodingFailed         = false;
        std::size_t frameIndex             = 0;

        {
            // FIXME: framesPerSecond is a float, should be properly converted to rational
            VideoFileWriter f(tmpVideoFilename.string(), *encoder);
            logDebug("{}: New video file", tmpVideoFilename);

            for (; frameIndex < imageStream.framesPerFile; frameIndex++)
            {
                ImageStream::Image img;
                imageStream.pop(img);
                if (img.data.empty())
                {
                    imageStreamClosedEarly = true;
                    break;
                }

                try
                {
                    f.write(img);
                }
                catch (const std::exception& e)
                {
                    logCritical("{}: Failed to encode frame {}: {}",
                                tmpVideoFilename,
                                frameIndex,
                                e.what());
                    encodingFailed = true;
                    break;
                }

                if (frameIndex % debugInterval == 0)
                {
                    logDebug("{}: Wrote video frame {}", tmpVideoFilename, frameIndex);
                }

                if (!startFrameTime)
                {
                    startFrameTime = img.timestamp;
                }
                endFrameTime = img.timestamp;

                if (frameTimestamps.is_open())
                {
                    frameTimestamps << fmt::format("{}_{:.6}\n", imageStream.id, img.timestamp);
                    frameTimestamps.flush();
                }
            }

            if (!encodingFailed)
            {
                try
                {
                    f.close();
                }
                catch (const std::exception& e)
                {
                    logCritical("{}: Failed to finalize video stream: {}",
                                tmpVideoFilename,
                                e.what());
                    encodingFailed = true;
                }
            }
        } // VideoFileWriter destroyed here; the shared encoder is preserved.

        frameTimestamps.close();

        // An encoding error may have left the shared encoder session in an undefined state. Drop
        // it so a fresh session is created for the next file instead of propagating the fault to
        // every subsequent file. The partial tmp file is left behind and not promoted to output.
        if (encodingFailed)
        {
            encoders.erase(imageStream.id);
            logWarning("{}: Discarded video file and reset encoder after encoding error",
                       tmpVideoFilename);
            continue;
        }

        if (!imageStreamClosedEarly)
        {
            try
            {
                const auto outDir = fs::path{settings.outputDirectory()} / imageStream.id;
                if (!fs::exists(outDir))
                {
                    fs::create_directories(outDir);
                    fs::permissions(outDir,
                                    fs::owner_all | fs::group_all | fs::others_read |
                                        fs::others_exe);
                }

                const auto outVideoFilename = outDir / fmt::format("{}_{:.6}--{:.6}.mp4",
                                                                   imageStream.id,
                                                                   *startFrameTime,
                                                                   *endFrameTime);

                fs::rename(tmpVideoFilename, outVideoFilename);
                fs::permissions(outVideoFilename,
                                fs::owner_read | fs::owner_write | fs::group_read |
                                    fs::group_write | fs::others_read | fs::others_write);

                const auto outFrameTimestampsFilename = outDir / fmt::format("{}_{:.6}--{:.6}.txt",
                                                                             imageStream.id,
                                                                             *startFrameTime,
                                                                             *endFrameTime);

                fs::rename(tmpFrameTimestampsFilename, outFrameTimestampsFilename);
                fs::permissions(outFrameTimestampsFilename,
                                fs::owner_read | fs::owner_write | fs::group_read |
                                    fs::group_write | fs::others_read | fs::others_write);

                logInfo("{}: Finished", outVideoFilename);
            }
            catch (const fs::filesystem_error& e)
            {
                logCritical("{}: Failed to finalize video file: {}", tmpVideoFilename, e.what());
            }
        }
        else
        {
            logInfo("{}: Interrupted", tmpVideoFilename);
        }
    }
}

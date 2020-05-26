// SPDX-License-Identifier: GPL-3.0-or-later

#include "Settings.h"

#include <fstream>

#include "boost/program_options.hpp"

#include <boost/property_tree/ptree.hpp>

#include <QStandardPaths>
#include <QDir>

#include "format.h"
#include "log.h"

#if defined(USE_BASLER) && USE_BASLER
    #include "camera/BaslerCamera.h"
#endif

template<typename T>
struct dependent_false : std::false_type
{
};

const Settings& Settings::instance()
{
    static const Settings instance;
    return instance;
}

Settings::Settings()
{
    const auto configLocation = QDir(
        QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
    if (!configLocation.exists())
    {
        configLocation.mkpath(".");
    }

    const auto configFilename = configLocation.absoluteFilePath("config.json").toStdString();

    std::fstream configFile;
    configFile.open(configFilename, std::ios::in);
    if (configFile.good())
    {
        configFile.imbue(std::locale::classic());
        boost::property_tree::read_json(configFile, _tree);
        loadNewSettings();
    }
    else
    {
        configFile.open(configFilename, std::ios::out | std::ios::trunc);
        configFile.imbue(std::locale::classic());
        _tree = detectSettings();
        boost::property_tree::write_json(configFile, _tree);
        logInfo("New settings file created: Edit it manually and run application again: {}",
                configFilename);
        std::exit(0);
    }
}

const boost::property_tree::ptree Settings::detectSettings() const
{
    auto tree = boost::property_tree::ptree{};

    tree.put("tmp_directory", "data/tmp");
    tree.put("out_directory", "data/out");

    auto& videoEncoders = tree.put_child("video_encoders", {});
    videoEncoders.put("h265_sw_0", "libx265");

    auto& videoStreams = tree.put_child("video_streams", {});

    std::size_t camIndex = 0;

    auto addVideoStreamForCameraConfig = [&camIndex, &videoStreams](VideoStream::Camera config) {
        auto& videoStreamTree = videoStreams.put_child(fmt::format("Cam-{}", camIndex++), {});

        auto& cameraTree = videoStreamTree.put_child("camera", {});
        cameraTree.put("backend", config.backend);
        cameraTree.put("serial", config.serial);

        cameraTree.put("offset_x", config.offset_x);
        cameraTree.put("offset_y", config.offset_y);
        cameraTree.put("width", config.width);
        cameraTree.put("height", config.height);

        auto& triggerTree = cameraTree.put_child("trigger", {});

        std::visit(
            [&](auto&& value) {
                using T = std::decay_t<decltype(value)>;

                if constexpr (std::is_same_v<T, VideoStream::Camera::HardwareTrigger>)
                {
                    triggerTree.put("type", "hardware");
                    triggerTree.put("source", value.source);

                    videoStreamTree.put("frames_per_second", "FRAMES_PER_SECOND");
                }
                else if constexpr (std::is_same_v<T, VideoStream::Camera::SoftwareTrigger>)
                {
                    triggerTree.put("type", "software");
                    triggerTree.put("frames_per_second", value.framesPerSecond);

                    videoStreamTree.put("frames_per_second", value.framesPerSecond);
                }
                else
                    static_assert(dependent_false<T>::value);
            },
            config.trigger);

        videoStreamTree.put("frames_per_file", 500);

        auto& encoderTree = videoStreamTree.put_child("encoder", {});

        encoderTree.put("id", "h265_sw_0");

        auto& encoderOptions = encoderTree.put_child("options", {});
        encoderOptions.put("preset", "5");
        encoderOptions.put("x265-params", "log-level=error");
    };

#if defined(USE_BASLER) && USE_BASLER
    for (auto config : BaslerCamera::getAvailable())
    {
        addVideoStreamForCameraConfig(config);
    }
#endif

    tree.put_child("video_streams", videoStreams);

    return tree;
}

template<typename T>
auto parseParam(const boost::property_tree::ptree& tree, const std::string& name)
    -> std::optional<Settings::VideoStream::Camera::Parameter<T>>
{
    if (auto strValue = tree.get_optional<std::string>(name); strValue)
    {
        if (*strValue == "auto")
        {
            return Settings::VideoStream::Camera::Parameter_Auto{};
        }
        else if (auto value = tree.get_optional<T>(name); value)
        {
            return Settings::VideoStream::Camera::Parameter_Manual<T>{*value};
        }

        throw std::runtime_error(
            fmt::format("Invalid value for camera parameter \'{}\': Expected \"auto\" or "
                        "appropriately typed value",
                        name));
    }

    return {};
};

void Settings::loadNewSettings()
{
    for (const auto& [videoStreamId, videoStreamTree] : _tree.get_child("video_streams"))
    {
        VideoStream stream;

        stream.id = QString::fromStdString(videoStreamId);

        const auto& cameraTree = videoStreamTree.get_child("camera");

        stream.camera.backend = cameraTree.get<std::string>("backend");
        stream.camera.serial  = cameraTree.get<std::string>("serial");

        stream.camera.offset_x = cameraTree.get<int>("offset_x");
        stream.camera.offset_y = cameraTree.get<int>("offset_y");
        stream.camera.width    = cameraTree.get<int>("width");
        stream.camera.height   = cameraTree.get<int>("height");

        const auto& triggerTree = cameraTree.get_child("trigger");

        const auto triggerType = triggerTree.get<std::string>("type");
        if (triggerType == "hardware")
        {
            VideoStream::Camera::HardwareTrigger trigger;
            trigger.source        = triggerTree.get<int>("source");
            stream.camera.trigger = trigger;
        }
        else if (triggerType == "software")
        {
            VideoStream::Camera::SoftwareTrigger trigger;
            trigger.framesPerSecond = triggerTree.get<float>("frames_per_second");
            stream.camera.trigger   = trigger;
        }
        else
        {
            throw std::runtime_error(fmt::format("Invalid camera trigger type: {}", triggerType));
        }

        stream.camera.buffer_size      = cameraTree.get_optional<int>("buffer_size");
        stream.camera.throughput_limit = cameraTree.get_optional<int>("throughput_limit");

        const auto parseAutoIntParam =
            [](const boost::property_tree::ptree& tree,
               const std::string&                 name) -> boost::optional<boost::optional<int>> {
            if (auto strValue = tree.get_optional<std::string>(name); strValue)
            {
                if (*strValue == "auto")
                {
                    return {boost::none};
                }
                else if (auto intValue = tree.get_optional<int>(name); intValue)
                {
                    return {{intValue}};
                }

                throw std::runtime_error(
                    fmt::format("Invalid value for camera parameter \'{}\': Expected \"auto\" or "
                                "integer value",
                                name));
            }

            return boost::none;
        };

        stream.camera.blacklevel = parseParam<float>(cameraTree, "blacklevel");
        stream.camera.exposure   = parseParam<float>(cameraTree, "exposure");
        stream.camera.gain       = parseParam<float>(cameraTree, "gain");

        stream.framesPerSecond = videoStreamTree.get<float>("frames_per_second");
        stream.framesPerFile   = videoStreamTree.get<size_t>("frames_per_file");

        const auto& encoderTree = videoStreamTree.get_child("encoder");
        stream.encoder.id       = encoderTree.get<std::string>("id");

        for (const auto& [key, value] : encoderTree.get_child("options"))
        {
            stream.encoder.options.emplace(key, value.get_value<std::string>());
        }

        _videoStreams.push_back(stream);
    }

    for (auto& [id, name] : _tree.get_child("video_encoders"))
    {
        _videoEncoders.emplace(id, name.get_value<std::string>());
    }

    _tmpDirectory = _tree.get<std::string>("tmp_directory");
    _outDirectory = _tree.get<std::string>("out_directory");
}

const std::vector<Settings::VideoStream>& Settings::videoStreams() const
{
    return _videoStreams;
}

const std::unordered_map<std::string, std::string>& Settings::videoEncoders() const
{
    return _videoEncoders;
}

const std::string Settings::tmpDirectory() const
{
    return _tmpDirectory;
}

const std::string Settings::outDirectory() const
{
    return _outDirectory;
}
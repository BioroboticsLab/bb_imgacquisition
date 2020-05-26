// SPDX-License-Identifier: GPL-3.0-or-later

#include "Camera.h"

#include "format.h"

#if defined(USE_FLEA3) && USE_FLEA3
    #include "Flea3Camera.h"
#endif

#if defined(USE_XIMEA) && USE_XIMEA
    #include "XimeaCamera.h"
#endif

#if defined(USE_BASLER) && USE_BASLER
    #include "BaslerCamera.h"
#endif

Camera::Camera(Config config, VideoStream videoStream, Watchdog* watchdog)
: _config{config}
, _videoStream{videoStream}
, _watchdog{watchdog}
{
}

Camera::~Camera()
{
    // Signal stream end to blocking consumer thread
    _videoStream.push({});
}

Camera* Camera::make(Config config, VideoStream videoStream, Watchdog* watchdog)
{
#if defined(USE_FLEA3) && USE_FLEA3
    if (config.backend == "flea3")
    {
        return new Flea3Camera(config, videoStream, watchdog);
    }
#endif

#if defined(USE_XIMEA) && USE_XIMEA
    if (config.backend == "ximea")
    {
        return new XimeaCamera(config, videoStream, watchdog);
    }
#endif

#if defined(USE_BASLER) && USE_BASLER
    if (config.backend == "basler")
    {
        return new BaslerCamera(config, videoStream, watchdog);
    }
#endif

    throw std::runtime_error(fmt::format("No such camera backend: {}", config.backend));
}
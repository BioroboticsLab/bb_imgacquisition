// SPDX-License-Identifier: BSD-3-Clause
#pragma once

#include <memory>
#include <vector>
#include <map>

#include <QCoreApplication>

#include "camera/Camera.hpp"
#include "ImageStreamsWriter.hpp"
#include "Watchdog.hpp"

class ImgAcquisitionApp : public QCoreApplication
{
    Q_OBJECT

public:
    ImgAcquisitionApp(int& argc, char** argv);
    ~ImgAcquisitionApp();

private slots:
    void quit();

private:
    Watchdog _watchdog;

    std::vector<std::unique_ptr<Camera>> _cameraThreads;

    std::map<std::string, ImageStreamsWriter> _imageStreamsWriters;
};

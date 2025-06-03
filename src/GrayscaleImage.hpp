// SPDX-License-Identifier: BSD-3-Clause
#pragma once

#include <chrono>

#include <QVector>

struct GrayscaleImage
{
    std::chrono::system_clock::time_point timestamp;
    int                                   width;
    int                                   height;
    QVector<std::uint8_t>                 data;

    GrayscaleImage() = default;
    GrayscaleImage(int w, int h, std::chrono::system_clock::time_point t);
};

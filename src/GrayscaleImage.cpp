// SPDX-License-Identifier: BSD-3-Clause

#include "GrayscaleImage.hpp"

GrayscaleImage::GrayscaleImage(int w, int h, std::chrono::system_clock::time_point t)
: timestamp{t}
, width{w}
, height{h}
, data(width * height)
{
}

// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Buffer/MutexBuffer.h"
#include "Buffer/MutexLinkedList.h"

#include <QThread>

/**
 * @brief The VideoWriteThread class
 *
 * This class reads from two ringbuffers
 * and processes the leass occupied one
 * using the NvEncoder in HEVC.
 * This class should be spawned as a QThread.<br>
 * <br>
 * Warning:<br>
 * Keep in mind:<br>
 * -    The number of processors you spawn.
 *      Most consumer Nvidia GPU's are only able to spawn 2 HEVC GPU encoders.
 *      One thread will use one encoder and crash if no GPU encoder is available.<br>
 * -    The resolution. Encoding may throw errors madly if 4096x4096 is exceeded.
 *      This is a Nvidia GPU encoder limitation.
 */
class VideoWriteThread final : public QThread
{
    Q_OBJECT

public:
    /**
     * @brief _Buffer1 The first buffer to encode
     */
    MutexLinkedList* _Buffer1;

    /**
     * @brief _CamRing1 Cam number associated with the first ringbuffer
     */
    int _CamBuffer1;

    /**
     * @brief _Buffer2 The second buffer to encode
     */
    MutexLinkedList* _Buffer2;

    /**
     * @brief _CamRing2 Cam number associated with the second ringbuffer
     */
    int _CamBuffer2;

    /**
     * @brief Creates a new encoder glue. Initializes ringbuffers.
     */
    VideoWriteThread();

protected:
    /**
     * @brief Run the encoding thread.
     *
     * This is the function that will be iterated indefinitely.
     * The ringbuffers will be watched permanently and will be encoded.
     *
     * @param Length of the ringbuffer
     */
    void run();
};

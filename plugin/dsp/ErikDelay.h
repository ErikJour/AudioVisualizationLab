//
// Created by Erik Jourgensen on 8/3/26.
//

#pragma once
#include <vector>
#define MAX_DELAY_SECONDS 3


class ErikDelay {

public:
    ErikDelay();
    ~ErikDelay() = default;
    void init (double sampleRate);
    void reset();
    void setDelay(int delayInSamples);
    void processBuffer(float* buffer, int numSamples);
private:
    int                      mDelayBufferLength = 0;
    std::unique_ptr<float[]> mDelayBuffer         {};
    int                      mDelayInSamples    = 0;
    int                      mWritePosition     = 0;
    int                      mReadPosition      = 0;

};



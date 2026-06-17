//
// Created by Erik Jourgensen on 6/17/26.
//

#include "TrainingCircularBuffer.h"

namespace JourgensenTraining {

    TrainingCircularBuffer::TrainingCircularBuffer() :  mWritePosition(0),
                                                        mReadPosition(0),
                                                        mDelayInSamples(0) {}

    TrainingCircularBuffer::~TrainingCircularBuffer() = default;

    void TrainingCircularBuffer::init(double sampleRate)
    {
        mCircularBufferLength = static_cast<size_t>(sampleRate * MAX_BUFFER_TIME_SEC);
        mCircularBuffer = std::make_unique<float[]>(mCircularBufferLength);
        mWritePosition = 0;
    }

    void TrainingCircularBuffer::setDelay(int delayInSamples)
    {
        if (delayInSamples > static_cast<int>(mCircularBufferLength) - 1) { delayInSamples = static_cast<int>(mCircularBufferLength) - 1; }
        if (delayInSamples < 0) { delayInSamples = 0; }
        mDelayInSamples = delayInSamples;
        mReadPosition = mWritePosition - mDelayInSamples;
        if (mReadPosition < 0) { mReadPosition += static_cast<int>(mCircularBufferLength); }
    }
    void TrainingCircularBuffer::process(float* buffer, int numSamples)
    {
        for (int i = 0; i < numSamples; i++) {
            constexpr float wetMix = 0.5f;
            constexpr float dryMix = 0.5f;

            const float inputSample = buffer[i];
            mCircularBuffer[static_cast<size_t>(mWritePosition)] = inputSample;
            const float outputSample = mCircularBuffer[static_cast<size_t>(mReadPosition)];



            buffer[i] = (inputSample * dryMix) + (outputSample * wetMix);

            mWritePosition++;
            if (mWritePosition >= static_cast<int>(mCircularBufferLength)) { mWritePosition = 0; }
            mReadPosition++;
            if (mReadPosition >= static_cast<int>(mCircularBufferLength)) { mReadPosition = 0; }
        }
    }

}


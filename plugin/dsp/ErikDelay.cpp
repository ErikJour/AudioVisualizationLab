//
// Created by Erik Jourgensen on 8/3/26.
//

#include "ErikDelay.h"

ErikDelay::ErikDelay() {}

void ErikDelay::init (const double sampleRate)
{
    mDelayBufferLength = static_cast<int>(sampleRate) * MAX_DELAY_SECONDS;
    mDelayBuffer = std::make_unique<float>(mDelayBufferLength);
    reset();
}

void ErikDelay::reset()
{
    mWritePosition = 0;
    mReadPosition  = 0;
}

void ErikDelay::setDelay(int delayInSamples)
{
    if (delayInSamples < 0 )                      { delayInSamples = 0;                      }
    if (delayInSamples > mDelayBufferLength - 1 ) { delayInSamples = mDelayBufferLength - 1; }
    mDelayInSamples = delayInSamples;
    mReadPosition   = mReadPosition - mDelayInSamples;
    if ( mReadPosition > mDelayInSamples )        { mReadPosition += mDelayInSamples; }
}

void ErikDelay::processBuffer(float* buffer, const int numSamples) {
    for (int i = 0; i < numSamples; i++)
    {
        float inputSample = buffer[i];
        mDelayBuffer[mWritePosition] = inputSample;


    }
}
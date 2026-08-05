//
// Created by Erik Jourgensen on 8/3/26.
//

#include "ErikDelay.h"

#include <iostream>
#include <ostream>

ErikDelay::ErikDelay() {}

void ErikDelay::init (const double sampleRate)
{
    mDelayBufferLength = static_cast<int>(sampleRate) * MAX_DELAY_SECONDS;
    mDelayBuffer       = std::make_unique<float[]>(static_cast<size_t>(mDelayBufferLength));
    mWritePosition     = 0;
}

void ErikDelay::setDelay(int delayInSamples)
{
    if (delayInSamples < 0 )                      { delayInSamples = 0;                      }
    if (delayInSamples > mDelayBufferLength - 1 ) { delayInSamples = mDelayBufferLength - 1; }
    mDelayInSamples = delayInSamples;
    mReadPosition   = mReadPosition - mDelayInSamples;
    if ( mReadPosition < 0 )        { mReadPosition += mDelayBufferLength; }
}

void ErikDelay::processBuffer(float* buffer, const int numSamples) {
    for (int i = 0; i < numSamples; i++)
    {
        const float inputSample                                 = buffer[i];
        mDelayBuffer[static_cast<size_t>(mWritePosition)]       = inputSample;
        const float outputSample                                = mDelayBuffer[static_cast<size_t>(mReadPosition)];
        buffer[i]                                               = (inputSample * 0.5f) + (outputSample * 0.5f);
        //======================================================================================================
        //Feedback Loop
        //======================================================================================================
        mDelayBuffer[static_cast<size_t>(mWritePosition)]       = buffer[i];
        buffer[i]                                               = (inputSample * 0.5f) + (outputSample * 0.5f);
        //======================================================================================================
        mWritePosition++;
        if (mWritePosition >= mDelayBufferLength) { mWritePosition = 0; }
        mReadPosition++;
        if (mReadPosition >= mDelayBufferLength) { mReadPosition = 0; }
    }
}
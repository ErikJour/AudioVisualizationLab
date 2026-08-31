//
// Created by Erik Jourgensen on 6/1/26.
//

#include "TrainingFilter.h"

#include <complex>

TrainingFilter::TrainingFilter()
{
    mSampleRate = 44100;
}
TrainingFilter::~TrainingFilter() = default;

float TrainingFilter::filterOne(const float input)
{
    constexpr float freq = 10000.0f;
    const float tan_g    = std::tan((PI * freq / static_cast<float>(mSampleRate)));

    const float b0       = tan_g / (1.0f + tan_g);
    const float b1       = tan_g / (1.0f + tan_g);
    const float a1       = (tan_g - 1.0f) / (tan_g + 1.0f);

    const float output   = b0 * input + mDelay;
    mDelay               = b1 * input - a1 * output;

    return output;
}

float TrainingFilter::onePoleIIR(const float input, float delay, const float a0, const float b1)
{
    return a0 * input - b1 * delay;
}


void TrainingFilter::processBuffer(float* buffer, const int numSamples)
{
    for (int i = 0; i < numSamples; i++) {

        //=================================
        //One pole IIR
        //=================================
        // const float input       = buffer[i];
        // const float output      = onePoleIIR(input, mDelay, 0.05f, -0.95f);
        // mDelay                  = output; //FIR is input, IIR is output
        //=================================
        //Feedforward filter
        //=================================
        int delay = 0;
        if (buffer[i] < 1) { delay = 0;     }
        else               { delay = i - 1; }
        const float input          = buffer[i];
        const float delayedInput   = buffer[delay];
        const float output         = feedForwardFilter(input, delayedInput, 0.5f, 0.5f);
        //=================================
        //Buffer assignment
        //=================================
        buffer[i]               = output;

    }
}

float TrainingFilter::feedForwardFilter(float input, float delayedInput, float a0, float a1) {

    return (input * a0) + (delayedInput * a1);

}
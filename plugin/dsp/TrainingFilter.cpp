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

void TrainingFilter::processBuffer(float* buffer, int numSamples)
{
    const float freq = 10000.0f;

    for (int i = 0; i < numSamples; i++) {
        float input = buffer[i];
        const float tan = std::tan((PI * freq / static_cast<float>(mSampleRate)));
        const float a1 = (tan - 1) / (tan + 1);
        buffer[i] *= a1 * input + dn_1;
        dn_1 = input - a1 * buffer[i];
    }

}
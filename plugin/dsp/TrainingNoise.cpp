//
// Created by Erik Jourgensen on 6/1/26.
//

#include "TrainingNoise.h"

TrainingNoise::TrainingNoise()  = default;
TrainingNoise::~TrainingNoise() = default;

float TrainingNoise::getNextSample()
{
    const float output = (randomNoise.nextFloat() * 2.0f - 1.0f) * mLevel;
    return output;

}
void TrainingNoise::processBuffer(float* buffer, const int numSamples)
{
    for (int i = 0; i < numSamples; i++) {
        buffer[i] += getNextSample();
    }

}

//
// Created by Erik Jourgensen on 7/31/26.
//

#include "ErikOscillator.h"

#include <valarray>
#define PI 3.14159265359

ErikOscillator::ErikOscillator() :  mPhase(0.0),
                                    mSampleRate(44100),
                                    mIncrement(0.0f) {}

void ErikOscillator::reset() { mPhase = 0.0f; }

void ErikOscillator::setSampleRate(const double newSampleRate) { mSampleRate = newSampleRate; }

void ErikOscillator::setFrequency(const float frequency) { mIncrement = frequency / static_cast<float>(mSampleRate); }

float ErikOscillator::createSine() const {

    const float phaseInRadians = static_cast<float>(mPhase) * 2.0f * static_cast<float>(PI);
    return std::sin(phaseInRadians);
}

float ErikOscillator::processSample() {
    const float sample = createSine();
    mPhase += mIncrement;
    if (mPhase >= 1.0f) {
        mPhase -= 1.0f;
    }
    if (mPhase < 0.0f) {
        mPhase += 1.0f;
    }
    return sample * mGain;
}

void ErikOscillator::processBuffer(float* buffer, const int numSamples)
{
    for (int i = 0; i < numSamples; i++) {
        buffer[i] += processSample();
    }
}

float ErikOscillator::getPhase() const { return static_cast<float>(mPhase); }


//
// Created by Erik Jourgensen on 7/31/26.
//

#include "ErikOscillator.h"
#define PI 3.14159265359

ErikOscillator::ErikOscillator() :  mPhase(0.0),
                                    mSampleRate(44100),
                                    mIncrement(0.0f) {}

void ErikOscillator::reset() { mPhase = 0.0f; gainFactor = 1.0f;}

void ErikOscillator::setSampleRate(const double newSampleRate) { mSampleRate = newSampleRate; }

void ErikOscillator::setFrequency(const float frequency) { mIncrement = frequency / static_cast<float>(mSampleRate); }

float ErikOscillator::createSine() const {

    const float phaseInRadians = static_cast<float>(mPhase) * 2.0f * static_cast<float>(PI);
    return std::sinf(phaseInRadians);
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
        float input =  buffer[i];
        input += processSample();
        gainFactor *= 0.9999f;
        buffer[i] = input * gainFactor;
    }
}



float ErikOscillator::getPhase() const { return static_cast<float>(mPhase); }

//=================================================
//Fast sin function
//=================================================
float ErikOscillator::fastSin(const uint32_t phase)
{
    constexpr float frf3  = -1.0f / 6.0f;
    constexpr float frf5  = 1.0f / 120.0f;
    constexpr float frf7  = -1.0f / 5040.0f;
    constexpr float frf9  = 1.0f / 362880.0f;
    constexpr float f0pi5 = 1.570796327f;
    uint32_t tmp = 0x3f800000 | (phase >> 7);
    if (phase & 0x40000000)
        tmp ^= 0x007fffff;
    const float x         = (*reinterpret_cast<float *>(&tmp) - 1.0f) * f0pi5;
    const float x2        = x * x;
    const float asin      = ((((frf9 * x2 + frf7) * x2 + frf5) * x2 + frf3) * x2 + 1.0f) * x;
    return (phase & 0x80000000) ? -asin : asin;
}

// float ErikOscillator::fastsin2(uint32_t phase)
// {
//     const float frf3 = -1.0f / 6.0f;
//     const float frf5 = 1.0f / 120.0f;
//     const float frf7 = -1.0f / 5040.0f;
//     const float f0pi5 = 1.570796327f;
//     float x, x2, asin;
//     UINT32 tmp = 0x3f800000 | (phase >> 7);
//     if (phase & 0x40000000)
//         tmp ^= 0x007fffff;
//     x = (*((float*)&tmp) - 1.0f) * f0pi5;
//     x2 = x * x;
//     asin = (((frf7 * x2 + frf5) * x2 + frf3) * x2 + 1.0f) * x;
//     return (phase & 0x80000000) ? -asin : asin;
// }



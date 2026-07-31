//
// Created by Erik Jourgensen on 7/31/26.
//

#ifndef TRAININGHOUR_ERIKOSCILLATOR_H
#define TRAININGHOUR_ERIKOSCILLATOR_H
#include <cstdint>
#include <cmath>

class ErikOscillator {
public:
    ErikOscillator();
    ~ErikOscillator() = default;

    void reset();
    void setSampleRate(double newSampleRate);
    void setFrequency(float frequency);
    float createSine() const;
    float processSample();
    void processBuffer(float *buffer, int numSamples);
    float getPhase() const;
    //=================================================
    //Fast sin function
    //=================================================
    static float fastSin(uint32_t phase);


private:
    double mPhase      = 0.0;
    double mSampleRate = 44100.0;
    float  mIncrement  = 0.0f;
    float  mGain       = 0.5f;
};


#endif //TRAININGHOUR_ERIKOSCILLATOR_H
//
// Created by Erik Jourgensen on 6/1/26.
//

#ifndef TRAININGHOUR_TRAININGNOISE_H
#define TRAININGHOUR_TRAININGNOISE_H

#include <juce_audio_processors/juce_audio_processors.h>
#include <random>

class TrainingNoise {
    public:
        TrainingNoise();
        ~TrainingNoise();
        float getNextSample();
        void processBuffer(float* buffer, int numSamples);
    private:
        float mLevel { 0.25f };
        juce::Random randomNoise;
};


#endif //TRAININGHOUR_TRAININGNOISE_H
//
// Created by Erik Jourgensen on 6/1/26.
//

#ifndef TRAININGHOUR_TRAININGFILTER_H
#define TRAININGHOUR_TRAININGFILTER_H

constexpr float PI = 3.14159265358979323846f;


class TrainingFilter {

    public:
        TrainingFilter();
        ~TrainingFilter();
        float filterOne(float input);
        static float onePoleIIR(float input, float delay, float a0, float b1);
        float feedForwardFilter(float input, float delayedInput, float a0, float a1);

        void processBuffer(float *buffer, int numSamples);

    private:
        double mSampleRate { 44100.0 };
        float  dn_1        { 0.0     };
        float  mDelay      { 0.0f    };
};


#endif //TRAININGHOUR_TRAININGFILTER_H
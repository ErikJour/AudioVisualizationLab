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

        void processBuffer(float* buffer, int numSamples);

    private:
        double mSampleRate { 44100.0 };
        float dn_1 { 0.0 };
};


#endif //TRAININGHOUR_TRAININGFILTER_H
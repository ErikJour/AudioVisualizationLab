//
// Created by Erik Jourgensen on 6/17/26.
//

#ifndef TRAININGHOUR_TRAININGCIRCULARBUFFER_H
#define TRAININGHOUR_TRAININGCIRCULARBUFFER_H
#include <juce_audio_processors/juce_audio_processors.h>
#include <memory>


namespace JourgensenTraining {

    constexpr double MAX_BUFFER_TIME_SEC = 3.0;

    class TrainingCircularBuffer {
    public:
        TrainingCircularBuffer();
        ~TrainingCircularBuffer();
        void init(double sampleRate);
        void setDelay(int delayInSamples);
        void process(float* buffer, int numSamples);
    private:
        std::unique_ptr<float[]> mCircularBuffer;
        size_t mCircularBufferLength = 0;
        int mWritePosition = 0;
        int mReadPosition = 0;
        int mDelayInSamples = 0;

    };

}


#endif //TRAININGHOUR_TRAININGCIRCULARBUFFER_H
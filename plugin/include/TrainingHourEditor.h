#pragma once

#include "TrainingHourProcessor.h"

//==============================================================================
class TrainingHourProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit TrainingHourProcessorEditor (TrainingHourAudioProcessor&);
    ~TrainingHourProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TrainingHourAudioProcessor& processorRef;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrainingHourProcessorEditor)
};

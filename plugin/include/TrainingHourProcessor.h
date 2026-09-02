#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include "ErikDelay.h"
#include "ErikOscillator.h"
#include "TrainingNoise.h"
#include "TrainingFilter.h"

namespace ParameterID {
#define PARAMETER_ID(str) const juce::ParameterID str(#str, 1);
    //Params here
#undef PARAMETER_ID
}

//==============================================================================
class TrainingHourAudioProcessor final : public juce::AudioProcessor,
                                            public juce::ValueTree::Listener
{
public:
    //==============================================================================
    TrainingHourAudioProcessor();
    ~TrainingHourAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    float getSinePhase() const;
    //UI
    std::atomic<float> outputLevel = 0.0f;
    juce::AudioProcessorValueTreeState apvts {*this, nullptr, "Parameters", createParameterLayout() };


private:
    TrainingNoise noiseGenerator;
    TrainingFilter allPassFilter;
    ErikOscillator sineOsc;
    ErikDelay delay;

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    std::atomic<bool> parametersChanged{false};
    void valueTreePropertyChanged(juce::ValueTree&, const juce::Identifier&) override { parametersChanged.store(true); }
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrainingHourAudioProcessor)
};

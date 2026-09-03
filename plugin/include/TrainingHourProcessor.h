#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include "ErikDelay.h"
#include "ErikOscillator.h"
#include "TrainingNoise.h"
#include "TrainingFilter.h"
#include "MidiProcessor.h"
#include "AudioUtils.h"

namespace ParameterID {
#define PARAMETER_ID(str) const juce::ParameterID str(#str, 1);

    PARAMETER_ID(strikeNote)
    PARAMETER_ID(delaySamples)

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
    void setDelay(float input);
    //UI
    std::atomic<float> outputLevel = 0.0f;
    juce::AudioProcessorValueTreeState apvts {*this, nullptr, "Parameters", createParameterLayout() };
    bool mStrikeNote {false};



private:
    TrainingNoise              noiseGenerator;
    TrainingFilter             filterOne;
    ErikOscillator             sineOsc;
    ErikDelay                  delay;
    MidiProcessor              midiProcessor;
    juce::AudioParameterBool*  strikeNoteParam;
    juce::AudioParameterFloat* delaySamplesParam;


    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void valueTreePropertyChanged(juce::ValueTree&, const juce::Identifier&) override { parametersChanged.store(true); }
    void update();
    std::atomic<bool> parametersChanged{false};

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrainingHourAudioProcessor)
};

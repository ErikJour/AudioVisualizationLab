#pragma once

#include "TrainingHourProcessor.h"
#include "webGpuWindow.h"

//==============================================================================
class TrainingHourProcessorEditor final :   public juce::AudioProcessorEditor,
                                            private juce::Timer
{
public:
    explicit TrainingHourProcessorEditor (TrainingHourAudioProcessor&);
    ~TrainingHourProcessorEditor() override;

    //==============================================================================
    void parentHierarchyChanged();
    void resized() override;
    void timerCallback() override;

private:

    //======================================
    //Local Variables
    //=====================================
    TrainingHourAudioProcessor& processorRef;
    WebGpuWindow                mWebGpuWindow;
#if JUCE_MAC
    juce::NSViewComponent       mMetalView;
#endif
    double   mStartTimeMs         = 0.0;
    bool     mStartTimeSet        = false;
    uint32_t mConfiguredWidth     = 0;
    uint32_t mConfiguredHeight    = 0;
    float    phaseViz             = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrainingHourProcessorEditor)
};

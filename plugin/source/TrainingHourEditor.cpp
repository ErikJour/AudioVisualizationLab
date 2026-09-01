#include "TrainingHourProcessor.h"
#include "TrainingHourEditor.h"

//==============================================================================
TrainingHourProcessorEditor::TrainingHourProcessorEditor (TrainingHourAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    constexpr int initWidth  = 800;
    constexpr int initHeight = 450;
    setSize (initWidth, initHeight);
    mWebGpuWindow.initialize();
}

TrainingHourProcessorEditor::~TrainingHourProcessorEditor()
{
    stopTimer();
    mWebGpuWindow.terminate();
}

//==============================================================================
void TrainingHourProcessorEditor::parentHierarchyChanged()
{
    AudioProcessorEditor::parentHierarchyChanged();

    if (mWebGpuWindow.hasSurface() || getPeer() == nullptr)
        return;

    const auto* webGpuDisplay = juce::Desktop::getInstance().getDisplays().getPrimaryDisplay();
    const double scale        = webGpuDisplay ? webGpuDisplay->scale : 1.0;
    const auto width          = static_cast<uint32_t>(getWidth() * scale);
    const auto height         = static_cast<uint32_t>(getHeight() * scale);
    if (!mWebGpuWindow.initSurface(scale, width, height))
        return;
#if JUCE_MAC
    mMetalView.setInterceptsMouseClicks(false, false);
    addAndMakeVisible(&mMetalView);
    mMetalView.setView(mWebGpuWindow.getNativeView());
    mMetalView.setBounds(getLocalBounds());
#endif
    mStartTimeMs      = juce::Time::getMillisecondCounterHiRes();
    mStartTimeSet     = true;
    mConfiguredWidth  = width;
    mConfiguredHeight = height;
    startTimerHz(60);
    setResizable (false, false);
}
void TrainingHourProcessorEditor::resized()
{
   if (!mWebGpuWindow.hasSurface()) return;
#if JUCE_MAC
    mMetalView.setBounds(getLocalBounds());
    #endif
}

void TrainingHourProcessorEditor::timerCallback()
{
    if (mStartTimeSet) {
        // const double elapsed = (juce::Time::getMillisecondCounterHiRes() - mStartTimeMs) * 0.001;
        mWebGpuWindow.getScene().renderFrame(outputForScene);
    }

    outputForScene = processorRef.outputLevel.load(std::memory_order_relaxed);
}

void TrainingHourProcessorEditor::mouseDown(const juce::MouseEvent& event)
{
    const auto currentX = static_cast<float>(event.x);
    const auto currentY = static_cast<float>(event.y);
    std::cout << currentX << " " << currentY << std::endl;
}

void TrainingHourProcessorEditor::mouseUp  (const juce::MouseEvent& event)
{

}
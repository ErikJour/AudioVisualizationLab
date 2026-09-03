#include "TrainingHourProcessor.h"
#include "TrainingHourEditor.h"

//==============================================================================
TrainingHourProcessorEditor::TrainingHourProcessorEditor (TrainingHourAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    setSize (WIDTH, HEIGHT);
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
    const auto currentX    = static_cast<float>(event.x);
    const auto width       = static_cast<float>(getWidth());
    const auto height      = static_cast<float>(getHeight());
    const auto mappedX= juce::jmap<float> (currentX, 0, width, -1.0f, 1.0f);
    const auto currentY    = static_cast<float>(event.y);
    const auto mappedY= juce::jmap<float> (currentY, 0, height, 1.0f, -1.0f);

    mWebGpuWindow.getScene().setMouseCoords(mappedX, mappedY);
    mWebGpuWindow.getScene().setMouseDown(true);
    processorRef.mStrikeNote = true;

}

void TrainingHourProcessorEditor::mouseUp  (const juce::MouseEvent& event)
{
    juce::ignoreUnused(event);
    mWebGpuWindow.getScene().setMouseDown(false);
    processorRef.mStrikeNote = false;
}

void TrainingHourProcessorEditor::mouseDrag  (const juce::MouseEvent& event)
{
    //Get raw juce mouse values
    const int dragX            = event.getDistanceFromDragStartX();
    constexpr int minimumDragX = -40;
    constexpr int maxDragX     = 40;
    //Map these to -1 to 1
    const auto mappedX    = juce::jmap<float> (static_cast<float>(dragX),
                                                        minimumDragX,
                                                        maxDragX,
                                                        -1.0f,
                                                        1.0f);
    constexpr float minMapped  = -1.0f;
    constexpr float maxMapped  = 1.0f;

    if (mappedX <= maxMapped && mappedX >= minMapped) { mWebGpuWindow.getScene().buttonModCoords(mappedX); }

}
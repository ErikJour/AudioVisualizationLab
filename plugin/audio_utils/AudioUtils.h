//
// Created by Erik Jourgensen on 9/2/26.
//

#ifndef TRAININGHOUR_AUDIOUTILS_H
#define TRAININGHOUR_AUDIOUTILS_H
#include "juce_audio_processors/juce_audio_processors.h"

template<typename T>
inline static void castParameter(juce::AudioProcessorValueTreeState& apvts, const juce::ParameterID&, T& destination)
{
    destination = dynamic_cast(apvts.getParameter(id.getParamID()));
    jassert(destination);
}
#endif //TRAININGHOUR_AUDIOUTILS_H
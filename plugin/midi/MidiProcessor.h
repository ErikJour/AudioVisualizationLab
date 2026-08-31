//
// Created by Erik Jourgensen on 8/5/26.
//

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

class MidiProcessor {
public:
    void processMidi(juce::MidiBuffer& midiMessages) {
        processedMidi.clear();

        for (const juce::MidiMessageMetadata data : midiMessages)
        {
            auto message = data.getMessage();
            auto samplePos = data.samplePosition;
                juce::MidiMessage currentMessage = data.getMessage();



            if (message.isNoteOn()) {
                noteOn = true;
                mNote = currentMessage.getNoteNumber();
                std::cout << "Note: " << mNote << std::endl;
            }

            if (message.isNoteOff()) {
                    noteOn = false;
            }
            processedMidi.addEvent (message, samplePos);
         }
        midiMessages.swapWith (processedMidi);
    }
    bool noteOn = false;

private:
    juce::MidiBuffer processedMidi;
    int mNote = 0;
};


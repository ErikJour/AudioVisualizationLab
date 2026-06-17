juce_add_plugin(TrainingHour
        PLUGIN_NAME "Training Hour"
        COMPANY_NAME AnimatedInstruments
        IS_SYNTH true
        NEEDS_MIDI_INPUT TRUE
        NEEDS_MIDI_OUTPUT FALSE
        PLUGIN_MANUFACTURER_CODE JOUR
        PLUGIN_CODE TRHR
        FORMATS AU VST3 Standalone
        COPY_PLUGIN_AFTER_BUILD TRUE
)

set_target_properties(TrainingHour PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        COMPILE_WARNINGS_AS_ERROR ON
)

if (MSVC)
    target_compile_options(TrainingHour PRIVATE /W4)
else()
    target_compile_options(TrainingHour PRIVATE -Wall -Wextra -pedantic)
endif()

target_sources(TrainingHour
        PRIVATE
        source/TrainingHourEditor.cpp
        source/TrainingHourProcessor.cpp
        dsp/TrainingNoise.cpp
        dsp/TrainingFilter.cpp
        dsp/TrainingCircularBuffer.cpp
)

target_include_directories(TrainingHour
        PRIVATE
        include
        source
        dsp
)

target_link_libraries(TrainingHour
        PRIVATE
            juce::juce_audio_utils
            juce::juce_gui_extra
            juce::juce_dsp
        PUBLIC
            juce::juce_recommended_config_flags
            juce::juce_recommended_lto_flags
            juce::juce_recommended_warning_flags
)

target_compile_definitions(TrainingHour
        PUBLIC
            JUCE_USE_CURL=0
            JUCE_VST3_CAN_REPLACE_VST2=0
)


/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class SynthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SynthAudioProcessorEditor (SynthAudioProcessor&);
    ~SynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void findRes();

    void updateAngle();

private:

    float pi = 3.14159265359;

    float two_pi = 0;

    float exp = 0;

    float amp = 1;

    float cyclesPerSample = 0;

    float sampleRatePlugin = 48000;

    float angle = 0;

    float inc = 0;

    float frequency = 200;

    int samplesPerBlockPlugin = 0;

    bool note_on = false;

    float output = -1.0f;

    int sample_count = 0;

    float attack = 0;

    bool attack_active = false;

    float res = 0;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SynthAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthAudioProcessorEditor)
};

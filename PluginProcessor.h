/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "Note.h"
//==============================================================================
/**
*/
class SynthAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    SynthAudioProcessor();
    ~SynthAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    float getNextSample();

    void updateAngle();

    void findRes();
     
    void render(juce::AudioBuffer<float>& outputbuffer);

    void addVoice();

    void processMidi();

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

private:

    juce::MidiMessageSequence *track;

    double pi = 3.14159265359;

    float two_pi = 0.0f;

    float cyclesPerSample = 0.0f;

    int sampleRatePlugin = 0;

    float inc = 0.0f;

    float frequency = 0.0f;

    int samplesPerBlockPlugin = 0.0f;

    bool note_on = false;

    int sample_count = 0;

 //   float attack = 0.01;

    bool attack_active = false;

    float res = 0;
    
    juce::AudioBuffer<float> mix1; 

    juce::AudioBuffer<float> mix2;

    int totalOutputChannels = 0;

    static constexpr int MAX_VOICES = 8;

    std::array<Note, MAX_VOICES> voices;

    juce::MidiMessage noteMsg;

    bool release = false;

    bool attack_bool = false;

    float volume = 0;

    bool kill_note = false;

    float mixoutput = 0.0f;

    float input1 = 0.0f;

    float input2 = 0.0f;

    int num_voices = 8;

    juce::uint8 data0;
    juce::uint8 data1;
    juce::uint8 data2;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthAudioProcessor)
};

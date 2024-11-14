/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
SynthAudioProcessor::SynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

SynthAudioProcessor::~SynthAudioProcessor()
{

 


}

//==============================================================================
const juce::String SynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SynthAudioProcessor::acceptsMidi() const
{

    return true;

}

bool SynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void SynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{

    DBG("foo");

    for (int i = 0; i < num_voices; i++)
    {
        voices[i].resetVoice();
    }


    track = new juce::MidiMessageSequence;

    sampleRatePlugin = sampleRate;

    samplesPerBlockPlugin = samplesPerBlock;

    mix1 = juce::AudioBuffer<float>(getTotalNumOutputChannels(), samplesPerBlock);

    mix2 = juce::AudioBuffer<float>(getTotalNumOutputChannels(), samplesPerBlock);

    mix1.clear();

    mix2.clear();
    //   updateAngle();

   // findRes();

  
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void SynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SynthAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{

    juce::ScopedNoDenormals noDenormals;


    buffer.clear(0, 0, buffer.getNumSamples());

    mix1.clear(0, 0, buffer.getNumSamples());

    mix2.clear(0, 0, buffer.getNumSamples());

    samplesPerBlockPlugin = buffer.getNumSamples();

    for (const auto msg : midiMessages)
    {

        //const auto message = msg.getMessage();

        data0 = msg.data[0];
        data1 = msg.data[1];
        data2 = msg.data[2];

        char s[16];
        snprintf(s, 16, "%02hhX %02hhX %02hhX", data0, data1, data2);
        //   DBG(s);

        if (data0 == 0x90)
        {

            DBG("Note On");

            snprintf(s, 16, "%d", data1);

            DBG(s);

            for (int j = 0; j < num_voices; j++)
            {

                Note& voice = voices[j];


                if (!voice.isActive())
                {

                    voice.addVoice(data1, sampleRatePlugin);

                    break;

                }


            }


        }


        if (data0 == 0x80)
        {

            DBG("Note Off");

            snprintf(s, 16, "%d", data1);

            DBG(s);

            for (int j = 0; j < num_voices; j++)
            {
                Note& voice = voices[j];

                if (voice.noteNum() == data1)
                {

                    // voices[j]->setRelease(true);

                    voice.setActive(false);

                }

            }


        }



    }

        //leftBuffer = buffer.getWritePointer(0);
//        rightBuffer = buffer.getWritePointer(1);

       // processMidi();
        render(buffer);


        midiMessages.clear();
  

}


float SynthAudioProcessor::getNextSample()
{

    return 0;
}

void SynthAudioProcessor::updateAngle()
{

    two_pi = (float) pi * 2.0f;

    cyclesPerSample = frequency / sampleRatePlugin;

    inc = cyclesPerSample * two_pi;
        
}



/*void SynthAudioProcessor::findRes()
{

    angle = 0;

    sample_count = 0;

    res = 1.0f / (std::powf(2, 24) / 2.0f);

    int samp_int = 0;

    while (angle < (two_pi))
    {

        float currentSample = (float)std::cos(angle);

//        samp_int = (int)angle;

        if (currentSample >= 1)
        {


            break;
        }

        angle += inc;

        sample_count++;

    }

    angle = 0;

    attack = res * sample_count;

}
*/

void SynthAudioProcessor::render(juce::AudioBuffer<float>& outputbuffer)
{

    auto* leftBuffer = outputbuffer.getWritePointer(0);
    auto* rightBuffer = outputbuffer.getWritePointer(1);


    float output = 0.0f;


    for (int track = 0; track < num_voices; track++)
    {

        Note &voice = voices[track];

        if (voice.isActive())
        {

          //  float angle = voices[track].getAngle();

          //  frequency = noteMsg.getMidiNoteInHertz(voices[track].noteNum());

            updateAngle();


            

            for (auto sample = 0; sample < samplesPerBlockPlugin; ++sample)
            {
                output = voice.nextSample();

                leftBuffer[sample] += output;

                rightBuffer[sample] += output;

                if (voice.isAttack())
                {
                    output *= voices[track].getAttack();

                }


                /*            if (voices[track]->getRelease())
                            {


                                if (voices[track]->getRelease() > 0.0f)
                                {
                                    output *= voices[track]->getRelease();
                                }
                                else
                                {
                                    voices[track]->setRelease(false);
                                    kill_note = true;
                                }



                            }
                            */
                //mix1.setSample(0, sample, output);


            }



            //if (kill_note)
            //{

              //  voices.remove(track);

                //kill_note = false;
            //}




           /* float mixoutput = 0.0f;

            float input1 = 0.0f;
            float input2 = 0.0f;

            for (int i = 0; i < outputbuffer.getNumSamples(); i++)
            {

                float input1 = mix1.getSample(0, i);

                float input2 = mix2.getSample(0, i);

                float output = input1 + input2;

                output *= 0.6f;
    
                leftBuffer[i] = output;

                rightBuffer[i] = output;
                
            }
            */
        }
    }
}
    



void SynthAudioProcessor::addVoice()
{

    


}

void SynthAudioProcessor::processMidi()
{

    for (int i = 0; i < track->getNumEvents(); i++)
    {
        auto message = track->getEventPointer(i);

        auto msg = message->message;


        if (msg.isNoteOn())
        {


            for (int j = 0; j < num_voices; j++)
            {

                if (!voices[j].isActive())
                {

                    voices[j].addVoice(msg.getNoteNumber(), sampleRatePlugin);

                    break;

                }
            
            
            }


           // frequency = msg.getMidiNoteInHertz(msg.getNoteNumber());

            //voices.add(new Note);

            //auto object = voices.getLast();

            //object->setNote(msg.getNoteNumber());

            //object->setAttack(true);

        }


        if (msg.isNoteOff())
        {
        
        
            for (int j = 0; j < voices.size(); j++)
            {

                if (voices[j].noteNum() == msg.getNoteNumber())
                {

                   // voices[j]->setRelease(true);

                    voices[j].setActive(false);

                }

            }


        }

    }

    track->clear();

}


//==============================================================================
bool SynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SynthAudioProcessor::createEditor()
{
    return new SynthAudioProcessorEditor (*this);
}

//==============================================================================
void SynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SynthAudioProcessor();
}

/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class Note
{
public:
    //==============================================================================
    Note()
    {


    }
    
    ~Note()
    {

    }

    
    void setActive(bool value)
    {

        active = value;

    }


    bool isActive()
    {

        return active;
    }

    void setNote(int note)
    {

        noteNumber = note;
    }

    int noteNum()
    {

        
        return noteNumber;
    }
    
    float getAngle()
    {

        return angle;
    }

    void setAngle(float angleset)
    {

        angle = angleset;

    }


    bool isRelease()
    {

        return release;
    }

    void setRelease(bool data)
    {
        release = data;
    }

    bool getRelease()
    {
        level_release *= 0.99f;

        return level_release;
    }

    void setAttack(bool data)
    {
        attack = data;
    }

    bool isAttack()    {
        return attack;
    }



    void setVolume(float vol)
    {

        volume = vol;

    }


    float getVolume()
    {

        return volume;
    }

    float getAttack()
    {

        level *= 0.99f;

        float output = 1.0f - level;

        if (output > 0.0f)
        {
            return output;
        }

        else
        {
            attack = false;
            return 0.0f;
        }

    }

    void addVoice(int num_note, float sampleRate)
    {

        freq = (float) 440.0f * std::exp2((num_note - 69.0f) / 12.0f);

        cyclesPerSample = freq / sampleRate;

        inc = cyclesPerSample * juce::MathConstants<float>::twoPi;

        active = true;

        noteNumber = num_note;

        angle = 0;

        attack = true;

        level = 1.0f;

        level_release = 1.0f;

        volume = 1.0f;

    }

    void resetVoice()
    {

        active = false;

        noteNumber = 0;

        angle = 0;

        attack = true;

        level = 1.0f;

        level_release = 1.0f;

        volume = 1.0f;

    }

    float nextSample()
    {
       float output =  (float) std::sin(angle);

            angle += inc;

        if (angle >= juce::MathConstants<float>::twoPi)
        {

            angle -= juce::MathConstants<float>::twoPi;

        }

        return output;

    }

private:

    float inc = 0.0f;

    float cyclesPerSample = 0.0f;

    float freq = 0.0f;

    bool active = false;

    int noteNumber = 0;

    float angle = 0;
   
    bool release = false;

    bool attack = false;

    float volume = 1.0f;

    float level = 1.0f;

    float level_release = 1.0f;

    float env = 0;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Note)
};
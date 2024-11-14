/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthAudioProcessorEditor::SynthAudioProcessorEditor (SynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 600);
}

SynthAudioProcessorEditor::~SynthAudioProcessorEditor()
{
}

//==============================================================================
void SynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
//    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);

    float a, b, c, d, e;

   
    float rad = pi / 180;


    a = std::sin(rad * 0);
    b = std::sin(rad * 90);
    c = std::sin(rad * 180);
    d = std::sin(rad * 270);
    e = std::sin(rad * 360);

    g.setColour(juce::Colours::blue);

    int x = 0;
    int y = 1;

    exp = 1.0f;

    int freq = 3;

    int ycol = 0;

    int count = 0;

    float samples = 48000.0f / 4.0f;

    float inc = 1.0f / samples;
   

    inc = std::log(0.01);

    for (int i = 0; i < samples; i++)
    {

      


         exp = std::exp(inc * x);
     
         //exp = (int)exp - exp;

         count++;

         if (exp >= 1.0f)
         {

          //  break;
         
         }

         y = exp + 250;

         g.drawFittedText(juce::String(exp), 0, ycol, 100, 10, juce::Justification::centred, 1);

         ycol += 20;

         g.drawLine(x, y, x + 1, y, 0.6f);

         x++;

    }

    

    g.drawFittedText(juce::String(count),0, 200, 100, 10, juce::Justification::centred, 1);
  //  g.drawFittedText(juce::String(b), 0, 20, 100, 10, juce::Justification::centred, 1);
  //  g.drawFittedText(juce::String(c), 0, 40, 100, 10, juce::Justification::centred, 1);
  //  g.drawFittedText(juce::String(d), 0, 60, 100, 10, juce::Justification::centred, 1);
  //  g.drawFittedText(juce::String(e), 0, 80, 100, 10, juce::Justification::centred, 1);

  //  updateAngle();
  //  findRes();
  //  g.setColour(juce::Colours::white);

  //  g.drawFittedText(juce::String(sample_count), 0, 250, 100, 10, juce::Justification::centred, 1);
  //  g.drawFittedText(juce::String::formatted("%.8f", res), 0, 270, 100, 10, juce::Justification::centred, 1);

}

void SynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}


void SynthAudioProcessorEditor::findRes()
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
void SynthAudioProcessorEditor::updateAngle()
    {

        two_pi = pi * 2.0f;

        cyclesPerSample = frequency / sampleRatePlugin;

        inc = cyclesPerSample * two_pi;


    }

   // attack = res * sample_count;

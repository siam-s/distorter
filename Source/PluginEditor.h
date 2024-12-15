/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "DistortionSliderStyle.h"
#include "ThreeSliderStyle.h"
#include "TypeSliderStyle.h"

//==============================================================================
/**
*/
class SiamS_P05_DistortionAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SiamS_P05_DistortionAudioProcessorEditor (SiamS_P05_DistortionAudioProcessor&);
    ~SiamS_P05_DistortionAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SiamS_P05_DistortionAudioProcessor& audioProcessor;
    
    Slider distortionSlider, mixSlider, inputGainSlider, outputGainSlider, typeSlider;
    AudioProcessorValueTreeState::SliderAttachment distortionAttachment, mixAttachment, inputGainAttachment, outputGainAttachment, typeAttachment;
    DistortionSliderStyle distortionLookAndFeel;
    ThreeSliderStyle threeLookAndFeel;
    TypeSliderStyle typeLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SiamS_P05_DistortionAudioProcessorEditor)
};

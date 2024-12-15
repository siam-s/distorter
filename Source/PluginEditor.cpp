/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SiamS_P05_DistortionAudioProcessorEditor::SiamS_P05_DistortionAudioProcessorEditor(SiamS_P05_DistortionAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
    distortionAttachment(p.apvts, "distortion", distortionSlider),
    inputGainAttachment(p.apvts, "input", inputGainSlider),
    outputGainAttachment(p.apvts, "output", outputGainSlider),
    mixAttachment(p.apvts, "mix", mixSlider),
    typeAttachment(p.apvts, "type", typeSlider)
{
    distortionSlider.setLookAndFeel(&distortionLookAndFeel);
    for (auto* s : { &inputGainSlider, &outputGainSlider, &mixSlider }) {
        s->setLookAndFeel(&threeLookAndFeel);
    }
    typeSlider.setLookAndFeel(&typeLookAndFeel);
    for (auto* s : { &distortionSlider, &inputGainSlider, &outputGainSlider, &mixSlider, &typeSlider }) {
        s->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        s->setTextBoxStyle(juce::Slider::NoTextBox, false, 1, 1);
        addAndMakeVisible(s);
    }

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 500);
}

SiamS_P05_DistortionAudioProcessorEditor::~SiamS_P05_DistortionAudioProcessorEditor()
{
    distortionSlider.setLookAndFeel(nullptr);
    inputGainSlider.setLookAndFeel(nullptr);
    outputGainSlider.setLookAndFeel(nullptr);
    mixSlider.setLookAndFeel(nullptr);
    typeSlider.setLookAndFeel(nullptr);
}

//==============================================================================

void SiamS_P05_DistortionAudioProcessorEditor::paint(juce::Graphics& g)
{
    Image background = ImageCache::getFromMemory(BinaryData::Distorter_png, BinaryData::Distorter_pngSize);
    g.drawImageAt(background, 0, 0);
}

void SiamS_P05_DistortionAudioProcessorEditor::resized()
{
    distortionSlider.setBounds(125, 90, 150, 150);
    mixSlider.setBounds(150, 280, 100, 100);
    inputGainSlider.setBounds(30, 280, 100, 100);
    outputGainSlider.setBounds(270, 280, 100, 100);
    typeSlider.setBounds(170, 420, 60, 60);
}

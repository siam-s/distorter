/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Mu45FilterCalc/Mu45FilterCalc.h"

//==============================================================================
SiamS_P05_DistortionAudioProcessor::SiamS_P05_DistortionAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ), apvts(*this, nullptr, "APVTS", {
         std::make_unique<AudioParameterFloat>("distortion", "Distortion", NormalisableRange<float>(0.01f, 1.f, 0.01f, 1.f), 0.01f),
         std::make_unique<AudioParameterFloat>("mix", "Mix", NormalisableRange<float>(0.f, 1.f, 0.01f, 1.f), .5f),
         std::make_unique<AudioParameterFloat>("input", "Input", NormalisableRange<float>(-12.f, 12.f, 0.01f), 1.f),
         std::make_unique<AudioParameterFloat>("output", "output", NormalisableRange<float>(-12.f, 12.f, 0.01f), 1.f),
         std::make_unique<AudioParameterInt>("type", "Type", 0, 1, 0)
         })
#endif
{
}

SiamS_P05_DistortionAudioProcessor::~SiamS_P05_DistortionAudioProcessor()
{
}

//==============================================================================
const juce::String SiamS_P05_DistortionAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SiamS_P05_DistortionAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SiamS_P05_DistortionAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SiamS_P05_DistortionAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SiamS_P05_DistortionAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SiamS_P05_DistortionAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SiamS_P05_DistortionAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SiamS_P05_DistortionAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SiamS_P05_DistortionAudioProcessor::getProgramName (int index)
{
    return {};
}

void SiamS_P05_DistortionAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SiamS_P05_DistortionAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mFs = sampleRate;
}

void SiamS_P05_DistortionAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SiamS_P05_DistortionAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void SiamS_P05_DistortionAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    //would do calcAlgorithmParams(), but there is not that much DSP work, so more efficient to do here :)

    float distortion = *apvts.getRawParameterValue("distortion");
    float mix = *apvts.getRawParameterValue("mix");
    float inputGain = *apvts.getRawParameterValue("input");
    inputGain = powf(10.0f, inputGain / 20.f);
    float outputGain = *apvts.getRawParameterValue("output");
    outputGain = powf(10.0f, outputGain / 20.f);
    int type = *apvts.getRawParameterValue("type");

    float LPCoeffs[5];
    Mu45FilterCalc::calcCoeffsLPF(LPCoeffs, 7000, 1, mFs);

    LPFilter.setCoefficients(LPCoeffs[0], LPCoeffs[1], LPCoeffs[2], LPCoeffs[3], LPCoeffs[4]);


    for (int channel = 0; channel < totalNumInputChannels; ++channel) {
        auto* channelData = buffer.getWritePointer(channel);

        for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
            auto input = channelData[sample];
            float x = input * inputGain;
            if (type == 0) {
                //soft(er) clipping
                float threshold = (3.f / 2.f) - distortion;
                if (x <= -threshold)
                    x = -(2.f / 3.f);
                else if (x > -threshold || x < threshold) {
                    x = x - (powf(x, 3.f) / 3.f);
                }
                else if(x >= threshold)
                    x = 2.f / 3.f;
            }
            else {
                //hard clipping
                float threshold = 1 - distortion;

                if (x > threshold)
                {
                    x = 1.f;
                }
                else if (x < -threshold)
                {
                    x = -1.f;
                }
            }
            channelData[sample] = ((input * mix) + (x * (1 - mix))) * outputGain;
            LPFilter.tick(channelData[sample]);
        }
    }
}

//==============================================================================
bool SiamS_P05_DistortionAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SiamS_P05_DistortionAudioProcessor::createEditor()
{
    return new SiamS_P05_DistortionAudioProcessorEditor (*this);
}

//==============================================================================
void SiamS_P05_DistortionAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    if (auto xmlState = apvts.copyState().createXml())
        copyXmlToBinary(*xmlState, destData);
}

void SiamS_P05_DistortionAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    if (auto xmlState = getXmlFromBinary(data, sizeInBytes))
        apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SiamS_P05_DistortionAudioProcessor();
}

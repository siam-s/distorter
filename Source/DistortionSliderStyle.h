/*
  ==============================================================================

    SliderStyle.h
    Created: 9 Dec 2024 4:09:25pm
    Author:  siami

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class DistortionSliderStyle : public LookAndFeel_V4
{
public:
    DistortionSliderStyle();
    ~DistortionSliderStyle();

    void drawRotarySlider(Graphics& g, 
                                int x, int y, int width, int height, float sliderPos,
                                float startAngle, float endAngle, Slider& slider) override;

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    Image sliderImage;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DistortionSliderStyle)
};
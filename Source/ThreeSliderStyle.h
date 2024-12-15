/*
  ==============================================================================

    ThreeSliderStyle.h
    Created: 12 Dec 2024 6:55:35pm
    Author:  siami

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class ThreeSliderStyle : public LookAndFeel_V4
{
public:
    ThreeSliderStyle();
    ~ThreeSliderStyle();

    void drawRotarySlider(Graphics& g,
        int x, int y, int width, int height, float sliderPos,
        float startAngle, float endAngle, Slider& slider) override;

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    Image sliderImage;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ThreeSliderStyle)
};
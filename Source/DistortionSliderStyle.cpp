/*
  ==============================================================================

    SliderStyle.cpp
    Created: 9 Dec 2024 4:09:44pm
    Author:  siami

  ==============================================================================
*/

#include "DistortionSliderStyle.h"

DistortionSliderStyle::DistortionSliderStyle()
{
    sliderImage = ImageCache::getFromMemory(BinaryData::distortionslider_png, BinaryData::distortionslider_pngSize);
}

DistortionSliderStyle::~DistortionSliderStyle()
{

}

//==============================================================================

void DistortionSliderStyle::drawRotarySlider(Graphics& g,
                                               int x, int y, int width, int height, float sliderPos,
                                               float startAngle, float endAngle, Slider& slider)
{
    if (sliderImage.isValid()) 
    {
        const double rotation = (slider.getValue() - slider.getMinimum())
                                / (slider.getMaximum() - slider.getMinimum());
        const int frames = sliderImage.getHeight() / sliderImage.getWidth();
        const int frameId = (int)ceil(rotation * ((double)frames - 1.0));
        const float radius = jmin(width / 2.0f, height / 2.0f);
        const float centerX = x + width * 0.5f;
        const float centerY = y + height * 0.5f;
        const float rx = centerX - radius - 1.0f;
        const float ry = centerY - radius;

        int imgWidth = sliderImage.getWidth();
        int imgHeight = sliderImage.getHeight() / frames;

        g.drawImage(sliderImage, 0, 0, imgWidth, imgHeight, 0, frameId * imgHeight, imgWidth, imgHeight);
    }
    else
    {
        static const float textPpercent = 0.35f;
        Rectangle<float> text_bounds(1.0f + width * (1.0f - textPpercent) / 2.0f, 0.5f * height, width * textPpercent, 0.5f * height);

        g.setColour(Colours::white);

        g.drawFittedText(String("No Image"), text_bounds.getSmallestIntegerContainer(), Justification::horizontallyCentred | Justification::centred, 1);
    }

}

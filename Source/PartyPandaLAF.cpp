/*
  ==============================================================================

    PartyPandaLAF.cpp
    Created: 17 Feb 2024 8:07:00pm
    Author:  matta

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PartyPandaLAF.h"

PartyPandaLAF::PartyPandaLAF()
{
    // sliders
    setColour(juce::Slider::textBoxOutlineColourId, _red);
    setColour(juce::Slider::textBoxTextColourId, _white);

    // default window settings
    juce::Colour c = juce::Colour(255, 255, 255);
    setColour(juce::ResizableWindow::backgroundColourId, c);

    // labels
    setColour(juce::Label::textColourId, _blue);

    setDefaultSansSerifTypeface(_komikax.getTypefacePtr());

}

void PartyPandaLAF::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
{
    auto radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
    auto centreX = (float)x + (float)width * 0.5f;
    auto centreY = (float)y + (float)height * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    // fill
    g.setColour(_blue);
    g.fillEllipse(rx, ry, rw, rw);

    // outline
    g.setColour(_yellow);
    g.drawEllipse(rx, ry, rw, rw, 4.0f);

    juce::Path p;
    auto pointerLength = radius * 0.75f;
    auto pointerThickness = 3.0f;
    p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
    p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

    // pointer
    g.setColour(_white);
    g.fillPath(p);

    slider.setColour(juce::Slider::textBoxOutlineColourId, _red);
    slider.setColour(juce::Slider::textBoxBackgroundColourId, _red);
    slider.setColour(juce::Slider::textBoxTextColourId, _white);
}

juce::Label* PartyPandaLAF::createSliderTextBox(juce::Slider& slider)
{
    juce::Label* labelPtr = LookAndFeel_V4::createSliderTextBox(slider);
    labelPtr->setFont(25.0f);
    return labelPtr;
}

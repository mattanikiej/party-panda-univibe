/*
  ==============================================================================

    PartyPandaLAF.h
    Created: 17 Feb 2024 8:07:00pm
    Author:  matta

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class PartyPandaLAF  : public juce::LookAndFeel_V4
{
public:
    PartyPandaLAF();

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override;
    juce::Label* createSliderTextBox(juce::Slider& slider) override;

private:
    // colors
    juce::Colour _red = juce::Colour(255, 60, 114);
    juce::Colour _blue = juce::Colour(7, 176, 239);
    juce::Colour _yellow = juce::Colour(254, 236, 45);
    juce::Colour _white = juce::Colour(255, 255, 255);

    // typefaces
    juce::Font _komikax = juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::KOMIKAX_ttf, BinaryData::KOMIKAX_ttfSize));

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PartyPandaLAF)
};

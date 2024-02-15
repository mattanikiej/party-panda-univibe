/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

//==============================================================================
/**
*/
class PartyPandaAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Slider::Listener
{
public:
    PartyPandaAudioProcessorEditor (PartyPandaAudioProcessor& p, juce::AudioProcessorValueTreeState& vts);
    ~PartyPandaAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void sliderValueChanged(juce::Slider* slider);

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PartyPandaAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& _valueTreeState;

    juce::Label _frequencyLabel;
    juce::Slider _frequencySlider;
    std::unique_ptr<SliderAttachment> _frequencyAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PartyPandaAudioProcessorEditor)
};

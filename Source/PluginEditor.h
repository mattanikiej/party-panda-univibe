/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PartyPandaLAF.h"

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

    juce::Label _intensityLabel;
    juce::Slider _intensitySlider;
    std::unique_ptr<SliderAttachment> _intensityAttachment;

    juce::Label _rateLabel;
    juce::Slider _rateSlider;
    std::unique_ptr<SliderAttachment> _rateAttachment;

    juce::Label _depthLabel;
    juce::Slider _depthSlider;
    std::unique_ptr<SliderAttachment> _depthAttachment;

    juce::Label _throbLabel;
    juce::Slider _throbSlider;
    std::unique_ptr<SliderAttachment> _throbAttachment;

    juce::Label _wetLabel;
    juce::Slider _wetSlider;
    std::unique_ptr<SliderAttachment> _wetAttachment;

    juce::Label _dryLabel;
    juce::Slider _drySlider;
    std::unique_ptr<SliderAttachment> _dryAttachment;

    PartyPandaLAF _lookAndFeel;

    juce::Image _background = juce::ImageCache::getFromMemory(BinaryData::partypandabackground_png, BinaryData::partypandabackground_pngSize);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PartyPandaAudioProcessorEditor)
};

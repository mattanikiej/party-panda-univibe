/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PartyPandaAudioProcessorEditor::PartyPandaAudioProcessorEditor (PartyPandaAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p), _valueTreeState(vts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (700, 400);

    // effect sliders
    _intensityLabel.setText("Intensity", juce::dontSendNotification);
    addAndMakeVisible(_intensityLabel);

    addAndMakeVisible(_intensitySlider);
    _intensitySlider.setSliderStyle(juce::Slider::LinearBarVertical);
    _intensitySlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    _intensitySlider.setPopupDisplayEnabled(true, false, this);
    _intensitySlider.setTextValueSuffix(" - Intensity");
    _intensitySlider.addListener(this);
    _intensityAttachment.reset(new SliderAttachment(_valueTreeState, "intensity", _intensitySlider));

    _rateLabel.setText("Rate", juce::dontSendNotification);
    addAndMakeVisible(_rateLabel);

    addAndMakeVisible(_rateSlider);
    _rateSlider.setSliderStyle(juce::Slider::LinearBarVertical);
    _rateSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    _rateSlider.setPopupDisplayEnabled(true, false, this);
    _rateSlider.setTextValueSuffix(" - Rate");
    _rateSlider.addListener(this);
    _rateAttachment.reset(new SliderAttachment(_valueTreeState, "rate", _rateSlider));

    _depthLabel.setText("Depth", juce::dontSendNotification);
    addAndMakeVisible(_depthLabel);

    addAndMakeVisible(_depthSlider);
    _depthSlider.setSliderStyle(juce::Slider::LinearBarVertical);
    _depthSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    _depthSlider.setPopupDisplayEnabled(true, false, this);
    _depthSlider.setTextValueSuffix(" - Depth");
    _depthSlider.addListener(this);
    _depthAttachment.reset(new SliderAttachment(_valueTreeState, "depth", _depthSlider));

    _throbLabel.setText("Throb", juce::dontSendNotification);
    addAndMakeVisible(_throbLabel);

    addAndMakeVisible(_throbSlider);
    _throbSlider.setSliderStyle(juce::Slider::LinearBarVertical);
    _throbSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    _throbSlider.setPopupDisplayEnabled(true, false, this);
    _throbSlider.setTextValueSuffix(" - Throb");
    _throbSlider.addListener(this);
    _throbAttachment.reset(new SliderAttachment(_valueTreeState, "throb", _throbSlider));

    // dry wet sliders
    _wetLabel.setText("Wet", juce::dontSendNotification);
    addAndMakeVisible(_wetLabel);

    addAndMakeVisible(_wetSlider);
    _wetSlider.setSliderStyle(juce::Slider::LinearBarVertical);
    _wetSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    _wetSlider.setPopupDisplayEnabled(true, false, this);
    _wetSlider.setTextValueSuffix(" - wet");
    _wetSlider.addListener(this);
    _wetAttachment.reset(new SliderAttachment(_valueTreeState, "wet", _wetSlider));

    _dryLabel.setText("Dry", juce::dontSendNotification);
    addAndMakeVisible(_dryLabel);

    addAndMakeVisible(_drySlider);
    _drySlider.setSliderStyle(juce::Slider::LinearBarVertical);
    _drySlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    _drySlider.setPopupDisplayEnabled(true, false, this);
    _drySlider.setTextValueSuffix(" - dry");
    _drySlider.addListener(this);
    _dryAttachment.reset(new SliderAttachment(_valueTreeState, "dry", _drySlider));

}

PartyPandaAudioProcessorEditor::~PartyPandaAudioProcessorEditor()
{
}

//==============================================================================
void PartyPandaAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Party Panda", getLocalBounds(), juce::Justification::centred, 1);
}

void PartyPandaAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    _intensitySlider.setBounds(40, 30, 20, getHeight() - 60);
    _rateSlider.setBounds(80, 30, 20, getHeight() - 60);
    _depthSlider.setBounds(120, 30, 20, getHeight() - 60);
    _throbSlider.setBounds(160, 30, 20, getHeight() - 60);

    _drySlider.setBounds(200, 30, 20, getHeight() - 60);
    _wetSlider.setBounds(240, 30, 20, getHeight() - 60);
}

void PartyPandaAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    audioProcessor.setRate(_rateSlider.getValue());
}

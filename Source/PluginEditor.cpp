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

    _frequencyLabel.setText("Wet", juce::dontSendNotification);
    addAndMakeVisible(_frequencyLabel);

    addAndMakeVisible(_frequencySlider);
    _frequencySlider.setSliderStyle(juce::Slider::LinearBarVertical);
    _frequencySlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    _frequencySlider.setPopupDisplayEnabled(true, false, this);
    _frequencySlider.setTextValueSuffix(" - Frequency");
    _frequencySlider.addListener(this);
    _frequencyAttachment.reset(new SliderAttachment(_valueTreeState, "frequency", _frequencySlider));

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
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void PartyPandaAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    _frequencySlider.setBounds(40, 30, 20, getHeight() - 60);

}

void PartyPandaAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    audioProcessor.setFrequency(_frequencySlider.getValue());
}

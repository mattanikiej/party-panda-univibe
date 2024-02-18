/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

using Track = juce::Grid::TrackInfo;
using Fr = juce::Grid::Fr;
using Px = juce::Grid::Px;

//==============================================================================
PartyPandaAudioProcessorEditor::PartyPandaAudioProcessorEditor (PartyPandaAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p), _valueTreeState(vts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (700, 400);

    PartyPandaLAF::setDefaultLookAndFeel(&_lookAndFeel);

    // effect sliders
    _intensityLabel.setText("Intensity", juce::dontSendNotification);
    addAndMakeVisible(_intensityLabel);

    addAndMakeVisible(_intensitySlider);
    _intensitySlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    _intensitySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 150, 30);
    _intensitySlider.setPopupDisplayEnabled(true, false, this);
    _intensitySlider.setTextValueSuffix(" - Intensity");
    _intensitySlider.addListener(this);
    _intensityAttachment.reset(new SliderAttachment(_valueTreeState, "intensity", _intensitySlider));

    _rateLabel.setText("Rate", juce::dontSendNotification);
    addAndMakeVisible(_rateLabel);

    addAndMakeVisible(_rateSlider);
    _rateSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    _rateSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 150, 30);
    _rateSlider.setPopupDisplayEnabled(true, false, this);
    _rateSlider.setTextValueSuffix(" - Rate");
    _rateSlider.addListener(this);
    _rateAttachment.reset(new SliderAttachment(_valueTreeState, "rate", _rateSlider));

    _depthLabel.setText("Depth", juce::dontSendNotification);
    addAndMakeVisible(_depthLabel);

    addAndMakeVisible(_depthSlider);
    _depthSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    _depthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 150, 30);
    _depthSlider.setPopupDisplayEnabled(true, false, this);
    _depthSlider.setTextValueSuffix(" - Depth");
    _depthSlider.addListener(this);
    _depthAttachment.reset(new SliderAttachment(_valueTreeState, "depth", _depthSlider));

    _throbLabel.setText("Throb", juce::dontSendNotification);
    addAndMakeVisible(_throbLabel);

    addAndMakeVisible(_throbSlider);
    _throbSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    _throbSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 150, 30);
    _throbSlider.setPopupDisplayEnabled(true, false, this);
    _throbSlider.setTextValueSuffix(" - Throb");
    _throbSlider.addListener(this);
    _throbAttachment.reset(new SliderAttachment(_valueTreeState, "throb", _throbSlider));

    // dry wet sliders
    _wetLabel.setText("Wet", juce::dontSendNotification);
    addAndMakeVisible(_wetLabel);

    addAndMakeVisible(_wetSlider);
    _wetSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    _wetSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 150, 30);
    _wetSlider.setPopupDisplayEnabled(true, false, this);
    _wetSlider.setTextValueSuffix(" - wet");
    _wetSlider.addListener(this);
    _wetAttachment.reset(new SliderAttachment(_valueTreeState, "wet", _wetSlider));

    _dryLabel.setText("Dry", juce::dontSendNotification);
    addAndMakeVisible(_dryLabel);

    addAndMakeVisible(_drySlider);
    _drySlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    _drySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 150, 30);
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
    g.drawImageAt(_background, 0, 0);
}

void PartyPandaAudioProcessorEditor::resized()
{
    float sliderWidth = 130.0f;

    juce::Grid grid;

    grid.templateRows = { Track(Fr(1)), Track(Fr(1)) };
    grid.templateColumns = { Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1)) };

    grid.justifyContent = juce::Grid::JustifyContent::center;
    grid.justifyItems = juce::Grid::JustifyItems::center;

    grid.alignContent = juce::Grid::AlignContent::center;
    grid.alignItems = juce::Grid::AlignItems::center;

    grid.items = {
        // volume
        juce::GridItem(_drySlider).withWidth(sliderWidth).withHeight(sliderWidth),
        juce::GridItem(),
        juce::GridItem(),
        juce::GridItem(_wetSlider).withWidth(sliderWidth).withHeight(sliderWidth),

        // phase
        juce::GridItem(_intensitySlider).withWidth(sliderWidth).withHeight(sliderWidth),
        juce::GridItem(_rateSlider).withWidth(sliderWidth).withHeight(sliderWidth),

        // vibrato
        juce::GridItem(_depthSlider).withWidth(sliderWidth).withHeight(sliderWidth),
        juce::GridItem(_throbSlider).withWidth(sliderWidth).withHeight(sliderWidth),  
    };

    grid.performLayout(getLocalBounds());
}

void PartyPandaAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    audioProcessor.setRate(_rateSlider.getValue());
    audioProcessor.setThrob(_throbSlider.getValue());
    audioProcessor.setIntensity(_intensitySlider.getValue());
}

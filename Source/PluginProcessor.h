/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class PartyPandaAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    PartyPandaAudioProcessor();
    ~PartyPandaAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void setFrequencyByChannel(float frequency, int channel);

    void setRate(float rate);
    void setThrob(float throb);
    void setIntensity(float intensity);

private:
    const float pi = std::acosf(-1);
    const float twoPi = 2.0f * pi;

    juce::AudioProcessorValueTreeState _parameters;

    std::atomic<float>* _intensity = nullptr; // range of frequencies phaser will oscilate
    std::atomic<float>* _rate = nullptr; // speed at which phase will oscilate through frequencies
    std::atomic<float>* _depth = nullptr; // depth of vibrato effect
    std::atomic<float>* _throb = nullptr; // speed at which vibrato throbs

    std::atomic<float>* _wet = nullptr; // amount of dry signal
    std::atomic<float>* _dry = nullptr; // amount of wet signal

    // initial 4 stage phase filter
    std::array<juce::dsp::IIR::Filter<float>, 2> _phaseStage1Filters;
    std::array<juce::dsp::IIR::Filter<float>, 2> _phaseStage2Filters;
    std::array<juce::dsp::IIR::Filter<float>, 2> _phaseStage3Filters;
    std::array<juce::dsp::IIR::Filter<float>, 2> _phaseStage4Filters;

    float _intensityMapped;

    float _phaseRate;
    std::array<float, 2> _frequencyRange = { 60.0f, 20000.0f };
    std::array<float, 2> _rateRange = { 20.0f, 1000.0f };

    float _throbRate;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PartyPandaAudioProcessor)
};

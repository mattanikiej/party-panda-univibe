/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PartyPandaAudioProcessor::PartyPandaAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    _parameters(*this, nullptr, juce::Identifier("Pandamonium"),
        {
            std::make_unique<juce::AudioParameterFloat>("intensity",          // parameterID
                                                         "Intensity",         // parameter name
                                                         0.0f,                // minimum value
                                                         1.0f,                // maximum value
                                                         0.5f),               // default value

            std::make_unique<juce::AudioParameterFloat>("rate",               // parameterID
                                                         "Rate",              // parameter name
                                                         0.0f,                // minimum value
                                                         10.0f,               // maximum value
                                                         5.0f),               // default value

            std::make_unique<juce::AudioParameterFloat>("depth",              // parameterID
                                                         "Depth",             // parameter name
                                                         0.0f,                // minimum value
                                                         1.0f,                // maximum value
                                                         0.5f),               // default value

            std::make_unique<juce::AudioParameterFloat>("throb",              // parameterID
                                                         "Throb",             // parameter name
                                                         1.0f,                // minimum value
                                                         10.0f,               // maximum value
                                                         5.0f),               // default value

            std::make_unique<juce::AudioParameterFloat>("wet",                // parameterID
                                                         "Wet",               // parameter name
                                                         0.0f,                // minimum value
                                                         10.0f,                // maximum value
                                                         1.0f),               // default value

            std::make_unique<juce::AudioParameterFloat>("dry",                // parameterID
                                                         "Dry",               // parameter name
                                                         0.0f,                // minimum value
                                                         10.0f,                // maximum value
                                                         1.0f),               // default value
        })
#endif
{
    _intensity = _parameters.getRawParameterValue("frequency");
    _rate = _parameters.getRawParameterValue("rate");
    _depth = _parameters.getRawParameterValue("depth");
    _throb = _parameters.getRawParameterValue("throb");

    _wet = _parameters.getRawParameterValue("wet");
    _dry = _parameters.getRawParameterValue("dry");

    setRate(*_rate);
}

PartyPandaAudioProcessor::~PartyPandaAudioProcessor()
{
}

//==============================================================================
const juce::String PartyPandaAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PartyPandaAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PartyPandaAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PartyPandaAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PartyPandaAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PartyPandaAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PartyPandaAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PartyPandaAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String PartyPandaAudioProcessor::getProgramName (int index)
{
    return {};
}

void PartyPandaAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PartyPandaAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumInputChannels();

    auto pf1 = juce::dsp::IIR::Coefficients<float>::makeAllPass(sampleRate, _frequencyRange[0]);
    auto pf2 = juce::dsp::IIR::Coefficients<float>::makeAllPass(sampleRate, _frequencyRange[0]);
    auto pf3 = juce::dsp::IIR::Coefficients<float>::makeAllPass(sampleRate, _frequencyRange[0]);
    auto pf4 = juce::dsp::IIR::Coefficients<float>::makeAllPass(sampleRate, _frequencyRange[0]);

    for (auto& f : _phaseStage1Filters)
    {
        f.prepare(spec);
        f.coefficients = pf1;
    }

    for (auto& f : _phaseStage2Filters)
    {
        f.prepare(spec);
        f.coefficients = pf2;
    }

    for (auto& f : _phaseStage3Filters)
    {
        f.prepare(spec);
        f.coefficients = pf3;
    }

    for (auto& f : _phaseStage4Filters)
    {
        f.prepare(spec);
        f.coefficients = pf4;
    }
    
}

void PartyPandaAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PartyPandaAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void PartyPandaAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        auto& phaseF1 = _phaseStage1Filters[channel];
        auto& phaseF2 = _phaseStage2Filters[channel];
        auto& phaseF3 = _phaseStage3Filters[channel];
        auto& phaseF4 = _phaseStage4Filters[channel];

        int stepsTaken = 0;
        float phaseFUpdates = 0.0f;

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            auto dry = channelData[sample];
            auto wet = dry;

            // update phase oscillator if needed
            if (stepsTaken == 20)
            {
                stepsTaken = 0;

                phaseFUpdates++;
                // reset phaseFUpdates to prevent overflow
                if (phaseFUpdates >= 360.0f) 
                {
                    phaseFUpdates = 0.0f;
                }

                // get phase step in radians
                float phaseStep = phaseFUpdates / 360.0f;
                phaseStep *= twoPi;
                
                auto newFrequency = std::sinf(phaseStep);
                newFrequency = juce::jmap(newFrequency, -1.0f, 1.0f, _frequencyRange[0], _frequencyRange[1]);

                setFrequencyByChannel(newFrequency, channel);
            }

            // initially pass through 4 stage phase filter
            wet = phaseF1.processSample(wet);
            wet += phaseF2.processSample(wet);
            wet += phaseF3.processSample(wet);
            wet += phaseF4.processSample(wet);
            // wet *= (*_intensity);
            
            float dryMix = (*_dry / 10.0f) * dry;
            float wetMix = (*_wet / 10.0f) * wet;
            channelData[sample] = dryMix + wetMix;

            stepsTaken++;
        }
    }
}

//==============================================================================
bool PartyPandaAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PartyPandaAudioProcessor::createEditor()
{
    return new PartyPandaAudioProcessorEditor (*this, _parameters);
}

//==============================================================================
void PartyPandaAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = _parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void PartyPandaAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(_parameters.state.getType()))
            _parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PartyPandaAudioProcessor();
}

//==============================================================================
void PartyPandaAudioProcessor::setFrequencyByChannel(float frequency, int channel)
{
    auto sampleRate = getSampleRate();

    auto pf1 = juce::dsp::IIR::Coefficients<float>::makeAllPass(sampleRate, frequency);
    auto pf2 = juce::dsp::IIR::Coefficients<float>::makeAllPass(sampleRate, frequency);
    auto pf3 = juce::dsp::IIR::Coefficients<float>::makeAllPass(sampleRate, frequency);
    auto pf4 = juce::dsp::IIR::Coefficients<float>::makeAllPass(sampleRate, frequency);

    _phaseStage1Filters[channel].coefficients = pf1;
    _phaseStage2Filters[channel].coefficients = pf2; 
    _phaseStage3Filters[channel].coefficients = pf3; 
    _phaseStage4Filters[channel].coefficients = pf4;
}

void PartyPandaAudioProcessor::setRate(float rate)
{
    float newRate = juce::jmap(rate, 0.0f, 10.0f, _rateRange[1], _rateRange[0]);
    _phaseRate = newRate;
}

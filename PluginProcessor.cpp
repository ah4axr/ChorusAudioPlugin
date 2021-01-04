/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AndrewHp04chorusAudioProcessor::AndrewHp04chorusAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    addParameter(mDelayMsecParam = new juce::AudioParameterFloat("Delay in M Sec",
        "Delay Time",
        7.0f,
        40.0f,
        10.0f
    ));
    addParameter(mLfoDepthParam = new juce::AudioParameterFloat("LFO Depth",
        "Depth",
        0.0f,
        20.0f,
        10.0f
    ));
    addParameter(mLfoSpeedParam = new juce::AudioParameterFloat("LFO Speed",
        "Speed",
        0.0f,
        20.0f,
        0.5f
    ));
    addParameter(mFeedbackDbParam = new juce::AudioParameterFloat("Feedback in db",
        "Feedback",
        0.0f,
        100.0f,
        50.0f
    ));
    addParameter(mPositiveParam = new juce::AudioParameterBool("Positive",
        "Positive",
        true
    ));
    addParameter(mNegativeParam = new juce::AudioParameterBool("Negative",
        "Negative",
        false
    ));
}

AndrewHp04chorusAudioProcessor::~AndrewHp04chorusAudioProcessor()
{
}

//==============================================================================
const juce::String AndrewHp04chorusAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AndrewHp04chorusAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AndrewHp04chorusAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AndrewHp04chorusAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AndrewHp04chorusAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AndrewHp04chorusAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AndrewHp04chorusAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AndrewHp04chorusAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AndrewHp04chorusAudioProcessor::getProgramName (int index)
{
    return {};
}

void AndrewHp04chorusAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

int AndrewHp04chorusAudioProcessor::calcMsecToSamps(float mSec)
{
    return mSec * 0.001 * mFs;
}

//==============================================================================
void AndrewHp04chorusAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    mFs = sampleRate;

    //initialize with max delay length
    int maxSamps = calcMsecToSamps(MAXDELAYMSEC);
    mDelay1L.setMaximumDelay(maxSamps);
    mDelay1R.setMaximumDelay(maxSamps);
}

void AndrewHp04chorusAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AndrewHp04chorusAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
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

void AndrewHp04chorusAudioProcessor::calcAlgorithmParams()
{
    // set the rate of the LFO
    mLFO.setFreq(mLfoSpeedParam->get(), mFs);

    //set feedback gain
    mFeedbackGainLin = mFeedbackDbParam->get() / 100;

    mPositive = mPositiveParam->get();
    mNegative = mNegativeParam->get();
}

void AndrewHp04chorusAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());


    auto* channelDataLeft = buffer.getWritePointer(0);
    auto* channelDataRight = buffer.getWritePointer(1);

    calcAlgorithmParams();

    float lfoOut, centerMidi, delayTime, outL, outR, tempL, tempR;
    long count = 0;
    int mod = 2;
    float coeffs[5];

    for (int samp = 0; samp < buffer.getNumSamples(); samp++)
    {
        outL = channelDataLeft[samp];
        outR = channelDataRight[samp];

        if (count % mod == 0)
        {
            // Update the LFO and use it's output to calc the new center freq
            //output between -1.0 and +1.0
            lfoOut = mLFO.tick();
            centerMidi = mDelayMsecParam->get() + mLfoDepthParam->get() * lfoOut;
            delayTime = calcMiditoHz(centerMidi);

            //check delay Time
            delayTime = (delayTime < 0.0) ? 0.0 : delayTime;
            //delayTime = (delayTime > 40.0) ? 40.0 : delayTime;

            mDelay1L.setDelay(delayTime);
            mDelay1R.setDelay(delayTime);

            count = 0;
        }
        count++;

        tempL = mDelay1L.nextOut();
        tempR = mDelay1R.nextOut();
        if (mPositive == true)
        {
            mDelay1L.tick(outL + mFeedbackGainLin * tempL);
            mDelay1R.tick(outR + mFeedbackGainLin * tempR);
        }
        else
        {
            mDelay1L.tick((outL + mFeedbackGainLin * tempL) * -1);
            mDelay1R.tick((outR + mFeedbackGainLin * tempR) * -1);
        }

        outL = channelDataLeft[samp] + tempL;
        outR = channelDataRight[samp] + tempR;

        channelDataLeft[samp] = outL;
        channelDataRight[samp] = outR;
    }
}

//==============================================================================
bool AndrewHp04chorusAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AndrewHp04chorusAudioProcessor::createEditor()
{
    return new AndrewHp04chorusAudioProcessorEditor (*this);
}

//==============================================================================
void AndrewHp04chorusAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AndrewHp04chorusAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AndrewHp04chorusAudioProcessor();
}

/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "StkLite-4.6.1/Delay.h"
#include "Mu45FilterCalc/Mu45FilterCalc.h"
#include "Mu45LFO/Mu45LFO.h"

#define MAXDELAYMSEC 10000.0

using namespace juce;
//==============================================================================
/**
*/
class AndrewHp04chorusAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    AndrewHp04chorusAudioProcessor();
    ~AndrewHp04chorusAudioProcessor() override;

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

private:
    //Functions
    //==============================================================================
    void calcAlgorithmParams();
    // convert MIDI Note Number to H
    float calcMiditoHz(float midiNote) { return 440.0 * powf(2, (midiNote - 60.0) / 12.0); }
    // convert Hz to MIDI Note Number
    float calcHzToMidi(float Hz) { return 12 * log2f(Hz / 440.0) + 60.0; }
    // milliseconds to seconds
    int calcMsecToSamps(float mSec);

    //User
    AudioParameterFloat* mCenterFreqParam;
    AudioParameterFloat* mLfoDepthParam;
    AudioParameterFloat* mLfoSpeedParam;

    AudioParameterFloat* mDelayMsecParam;
    AudioParameterFloat* mFeedbackDbParam;
    AudioParameterBool* mPositiveParam;
    AudioParameterBool* mNegativeParam;

    //Member Objects 
    Mu45LFO mLFO;
    stk::Delay mDelay1L;
    stk::Delay mDelay1R;

    float mFs;       // Sampling rate

    // Algorithm Parameters
    float mFeedbackGainLin;
    bool mPositive;
    bool mNegative;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AndrewHp04chorusAudioProcessor)
};

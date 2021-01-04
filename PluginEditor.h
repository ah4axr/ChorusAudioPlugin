/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class AndrewHp04chorusAudioProcessorEditor  : public juce::AudioProcessorEditor,
    public juce::Slider::Listener, public juce::Timer, public juce::Button::Listener
{
public:
    AndrewHp04chorusAudioProcessorEditor (AndrewHp04chorusAudioProcessor&);
    ~AndrewHp04chorusAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void sliderValueChanged(juce::Slider* slider) override;
    void timerCallback() override;
    void buttonClicked(juce::Button* button) override;
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AndrewHp04chorusAudioProcessor& audioProcessor;

    //Sliders
    juce::Slider mDelayMsecSlider;
    juce::Slider mLfoDepthSlider;
    juce::Slider mLfoSpeedSlider;
    juce::Slider mFeedbackDbSlider;

    //Labels
    juce::Label delayTimeLabel;
    juce::Label lfoDepthLabel;
    juce::Label lfoSpeedLabel;
    juce::Label feedbackLabel;

    juce::Label positiveLabel;
    juce::Label negativeLabel;

    juce::ToggleButton positive;
    juce::ToggleButton negative;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AndrewHp04chorusAudioProcessorEditor)
};

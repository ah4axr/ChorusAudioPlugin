/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

using namespace juce;
//==============================================================================
AndrewHp04chorusAudioProcessorEditor::AndrewHp04chorusAudioProcessorEditor (AndrewHp04chorusAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (500, 200);

    auto& params = processor.getParameters();

    ///////////////////////////////////////////////////////////////////////////////////////
    AudioParameterFloat* audioParam = (AudioParameterFloat*)params.getUnchecked(0);
    mDelayMsecSlider.setBounds(10, 75, 100, 100);
    mDelayMsecSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    mDelayMsecSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 20);
    mDelayMsecSlider.setRange(audioParam->range.start, audioParam->range.end);
    mDelayMsecSlider.setValue(*audioParam);
    mDelayMsecSlider.setTextValueSuffix(" ms");
    mDelayMsecSlider.addListener(this);
    addAndMakeVisible(mDelayMsecSlider);
    addAndMakeVisible(delayTimeLabel);
    delayTimeLabel.setText("Delay", juce::dontSendNotification);
    delayTimeLabel.setJustificationType(juce::Justification::centred);
    delayTimeLabel.attachToComponent(&mDelayMsecSlider, false);

    audioParam = (AudioParameterFloat*)params.getUnchecked(1);
    mLfoDepthSlider.setBounds(120, 75, 100, 100);
    mLfoDepthSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    mLfoDepthSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 20);
    mLfoDepthSlider.setRange(audioParam->range.start, audioParam->range.end);
    mLfoDepthSlider.setValue(*audioParam);
    //mLfoDepthSlider.setTextValueSuffix(" ms");
    mLfoDepthSlider.addListener(this);
    addAndMakeVisible(mLfoDepthSlider);
    addAndMakeVisible(lfoDepthLabel);
    lfoDepthLabel.setText("Depth", juce::dontSendNotification);
    lfoDepthLabel.setJustificationType(juce::Justification::centred);
    lfoDepthLabel.attachToComponent(&mLfoDepthSlider, false);

    audioParam = (AudioParameterFloat*)params.getUnchecked(2);
    mLfoSpeedSlider.setBounds(230, 75, 100, 100);
    mLfoSpeedSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    mLfoSpeedSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 20);
    mLfoSpeedSlider.setRange(audioParam->range.start, audioParam->range.end);
    mLfoSpeedSlider.setValue(*audioParam);
    //mLfoSpeedSlider.setTextValueSuffix(" ms");
    mLfoSpeedSlider.addListener(this);
    addAndMakeVisible(mLfoSpeedSlider);
    addAndMakeVisible(lfoSpeedLabel);
    lfoSpeedLabel.setText("Speed", juce::dontSendNotification);
    lfoSpeedLabel.setJustificationType(juce::Justification::centred);
    lfoSpeedLabel.attachToComponent(&mLfoSpeedSlider, false);

    audioParam = (AudioParameterFloat*)params.getUnchecked(3);
    mFeedbackDbSlider.setBounds(340, 75, 100, 100);
    mFeedbackDbSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    mFeedbackDbSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 20);
    mFeedbackDbSlider.setRange(audioParam->range.start, audioParam->range.end);
    mFeedbackDbSlider.setValue(*audioParam);
    //mLfoSpeedSlider.setTextValueSuffix(" ms");
    mFeedbackDbSlider.addListener(this);
    addAndMakeVisible(mFeedbackDbSlider);
    addAndMakeVisible(feedbackLabel);
    feedbackLabel.setText("Feedback", juce::dontSendNotification);
    feedbackLabel.setJustificationType(juce::Justification::centred);
    feedbackLabel.attachToComponent(&mFeedbackDbSlider, false);

    audioParam = (AudioParameterFloat*)params.getUnchecked(4);
    positive.setBounds(450, 50, 50, 50);
    positive.setToggleState(*audioParam, false);
    positive.addListener(this);
    positive.setRadioGroupId(1000);
    addAndMakeVisible(positive);
    addAndMakeVisible(positiveLabel);
    positiveLabel.setText("Positive", juce::dontSendNotification);
    positiveLabel.setJustificationType(juce::Justification::centred);
    positiveLabel.attachToComponent(&positive, false);

    audioParam = (AudioParameterFloat*)params.getUnchecked(5);
    negative.setBounds(450, 110, 50, 50);
    negative.setToggleState(*audioParam, false);
    negative.addListener(this);
    negative.setRadioGroupId(1000);
    addAndMakeVisible(negative);
    addAndMakeVisible(negativeLabel);
    negativeLabel.setText("Negative", juce::dontSendNotification);
    negativeLabel.setJustificationType(juce::Justification::centred);
    negativeLabel.attachToComponent(&negative, false);
}

AndrewHp04chorusAudioProcessorEditor::~AndrewHp04chorusAudioProcessorEditor()
{
}

//==============================================================================
void AndrewHp04chorusAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Chorus Plugin", getLocalBounds(), juce::Justification::centredTop, 1);
}

void AndrewHp04chorusAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void AndrewHp04chorusAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    auto& params = processor.getParameters();
    if (slider == &mDelayMsecSlider)
    {
        AudioParameterFloat* audioParam = (AudioParameterFloat*)params.getUnchecked(0);
        *audioParam = mDelayMsecSlider.getValue();
    }
    else  if (slider == &mLfoDepthSlider)
    {
        AudioParameterFloat* audioParam = (AudioParameterFloat*)params.getUnchecked(1);
        *audioParam = mLfoDepthSlider.getValue();
    }
    else  if (slider == &mLfoSpeedSlider)
    {
        AudioParameterFloat* audioParam = (AudioParameterFloat*)params.getUnchecked(2);
        *audioParam = mLfoSpeedSlider.getValue();
    }
    else  if (slider == &mFeedbackDbSlider)
    {
        AudioParameterFloat* audioParam = (AudioParameterFloat*)params.getUnchecked(3);
        *audioParam = mFeedbackDbSlider.getValue();
    }
}

void AndrewHp04chorusAudioProcessorEditor::timerCallback()
{
    auto& params = processor.getParameters();
    //Update the value of each slider to match the value in the Processor    
    AudioParameterFloat* delayTime = (AudioParameterFloat*)params.getUnchecked(0);
    mDelayMsecSlider.setValue(delayTime->get(), dontSendNotification);

    AudioParameterFloat* depth = (AudioParameterFloat*)params.getUnchecked(1);
    mLfoDepthSlider.setValue(depth->get(), dontSendNotification);

    AudioParameterFloat* speed = (AudioParameterFloat*)params.getUnchecked(2);
    mLfoSpeedSlider.setValue(speed->get(), dontSendNotification);

    AudioParameterFloat* feedback = (AudioParameterFloat*)params.getUnchecked(3);
    mFeedbackDbSlider.setValue(feedback->get(), dontSendNotification);
}

void AndrewHp04chorusAudioProcessorEditor::buttonClicked(Button* button)
{
    //auto state = button->getToggleState();
    //juce::String stateString = state ? "ON" : "OFF";
    auto& params = processor.getParameters();
    if (button == &positive)
    {
        AudioParameterBool* audioParam = (AudioParameterBool*)params.getUnchecked(4);
        *audioParam = positive.getToggleState();
    }
    else if (button == &negative)
    {
        AudioParameterBool* audioParam = (AudioParameterBool*)params.getUnchecked(5);
        *audioParam = negative.getToggleState();
    }
}
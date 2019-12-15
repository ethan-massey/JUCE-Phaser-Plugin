/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
EjmfinalphaserAudioProcessorEditor::EjmfinalphaserAudioProcessorEditor (EjmfinalphaserAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (405, 300);
    
    auto& processorParams = processor.getParameters();
    AudioParameterFloat* procParam; // pointer for Audioparameters
    
    // LFO RATE
    LFORateSlider.setBounds(0, 75, 100, 160);
    LFORateSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    LFORateSlider.setTextBoxStyle(Slider::TextBoxBelow, 1, 80, 30);
    procParam = (AudioParameterFloat*)processorParams.getUnchecked(0);
    LFORateSlider.setRange(procParam->range.start, procParam->range.end);
    LFORateSlider.setValue(*procParam);
    LFORateSlider.addListener(this);
    addAndMakeVisible(LFORateSlider);
    
    // LFO DEPTH
    LFODepthSlider.setBounds(125, 25, 175, 130);
    LFODepthSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    LFODepthSlider.setTextBoxStyle(Slider::TextBoxAbove, 1, 80, 30);
    procParam = (AudioParameterFloat*)processorParams.getUnchecked(1);
    LFODepthSlider.setRange(procParam->range.start, procParam->range.end);
    LFODepthSlider.setValue(*procParam);
    LFODepthSlider.addListener(this);
    addAndMakeVisible(LFODepthSlider);
    
    // FEEDBACK
    FeedbackSlider.setBounds(125, 125, 175, 130);
    FeedbackSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    FeedbackSlider.setTextBoxStyle(Slider::TextBoxBelow, 1, 80, 30);
    procParam = (AudioParameterFloat*)processorParams.getUnchecked(2);
    FeedbackSlider.setRange(procParam->range.start, procParam->range.end);
    FeedbackSlider.setValue(*procParam);
    FeedbackSlider.addListener(this);
    addAndMakeVisible(FeedbackSlider);
    
    // WET/DRY MIX
    WetDrySlider.setBounds(300, 75, 100, 160);
    WetDrySlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    WetDrySlider.setTextBoxStyle(Slider::TextBoxBelow, 1, 80, 30);
    procParam = (AudioParameterFloat*)processorParams.getUnchecked(3);
    WetDrySlider.setRange(procParam->range.start, procParam->range.end);
    WetDrySlider.setValue(*procParam);
    WetDrySlider.addListener(this);
    addAndMakeVisible(WetDrySlider);
}

void EjmfinalphaserAudioProcessorEditor::sliderValueChanged(Slider * slider) {
    auto& processorParams = processor.getParameters();
    
    if (slider == &LFORateSlider) {
        AudioParameterFloat* LFORateParam = (AudioParameterFloat*)processorParams.getUnchecked(0);
        float sliderValue = LFORateSlider.getValue();
        *LFORateParam = sliderValue;
    }else if(slider == &LFODepthSlider){
        AudioParameterFloat* LFODepthParam = (AudioParameterFloat*)processorParams.getUnchecked(1);
        float sliderValue = LFODepthSlider.getValue();
        *LFODepthParam = sliderValue;
    }else if(slider == &FeedbackSlider){
        AudioParameterFloat* feedbackParam = (AudioParameterFloat*)processorParams.getUnchecked(2);
        float sliderValue = FeedbackSlider.getValue();
        *feedbackParam = sliderValue;
    }else if(slider == &WetDrySlider){
        AudioParameterFloat* wetDryParam = (AudioParameterFloat*)processorParams.getUnchecked(3);
        float sliderValue = WetDrySlider.getValue();
        *wetDryParam = sliderValue;
    }
}

EjmfinalphaserAudioProcessorEditor::~EjmfinalphaserAudioProcessorEditor()
{
}

//==============================================================================
void EjmfinalphaserAudioProcessorEditor::paint (Graphics& g)
{

    g.setColour (Colours::white);
    g.setFont (15.0f);
    
    g.fillAll(Colours::lightslategrey);
    
    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawSingleLineText ("Rate (Hz)", 50, 50, Justification::centred);
    g.drawSingleLineText ("Depth", 210, 80, Justification::centred);
    g.drawSingleLineText ("Feedback (%)", 212, 210, Justification::centred);
    g.drawSingleLineText ("Wet/Dry Mix (%)", 350, 50, Justification::centred);
    
    g.setFont(20.0f);
    g.drawSingleLineText ("PHASER", 210, 145, Justification::centred);
}

void EjmfinalphaserAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Mu45FilterCalc/Mu45FilterCalc.h"
#include "Mu45LFO/Mu45LFO.h"
#include "StkLite-4.6.1/BiQuad.h"

//==============================================================================
/**
*/
class EjmfinalphaserAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    EjmfinalphaserAudioProcessor();
    ~EjmfinalphaserAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    void calcAlgorithmParams();
    
    // convert MIDI Note Number to Hz
    float calcMiditoHz(float midiNote){return 440.0*powf(2, (midiNote-60.0)/12.0);}

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EjmfinalphaserAudioProcessor)
    
    // User parameters
    AudioParameterFloat *mLFORate;
    AudioParameterFloat *mLFODepth;
    AudioParameterFloat *mFeedback;
    AudioParameterFloat *mWetDry;
    
    float mDryGain;
    float mWetGain;
    float mFs;
    
    float feedbackL;
    float feedbackR;
    
    float mMaxFc;
    float mMinFc;
    
    // LFO and All pass filters
    Mu45LFO mLFO1;
    Mu45LFO mLFO2;
    Mu45LFO mLFO3;
    
    stk::BiQuad mAPF1L, mAPF2L, mAPF3L, mAPF1R, mAPF2R, mAPF3R;
    
};

/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
EjmfinalphaserAudioProcessor::EjmfinalphaserAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    addParameter(mLFORate = new AudioParameterFloat("LFO Rate (Hz)", // parameterID,
                                                           "LFO Rate (Hz)", // parameterName,
                                                           0.5,    // minValue,
                                                           10.0, // maxValue,
                                                           1.25)); // defaultValue
    
    addParameter(mLFODepth = new AudioParameterFloat("LFO Depth", // parameterID,
                                                           "LFO Depth", // parameterName,
                                                           4.0,    // minValue,
                                                           24.0, // maxValue,
                                                           5.0)); // defaultValue
    
    addParameter(mFeedback = new AudioParameterFloat("Feedback (%)", // parameterID,
                                                           "Feedback (%)", // parameterName,
                                                           0.0,    // minValue,
                                                           90.0, // maxValue,
                                                           45.0)); // defaultValue
    
    addParameter(mWetDry = new AudioParameterFloat("Wet/Dry Mix (%)", // parameterID,
                                                           "Wet/Dry Mix (%)", // parameterName,
                                                           0.0,    // minValue,
                                                           100.0, // maxValue,
                                                           50.0)); // defaultValue
    

}

EjmfinalphaserAudioProcessor::~EjmfinalphaserAudioProcessor()
{
}

//==============================================================================
const String EjmfinalphaserAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool EjmfinalphaserAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool EjmfinalphaserAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool EjmfinalphaserAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double EjmfinalphaserAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int EjmfinalphaserAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int EjmfinalphaserAudioProcessor::getCurrentProgram()
{
    return 0;
}

void EjmfinalphaserAudioProcessor::setCurrentProgram (int index)
{
}

const String EjmfinalphaserAudioProcessor::getProgramName (int index)
{
    return {};
}

void EjmfinalphaserAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void EjmfinalphaserAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    mFs = sampleRate;
    mMaxFc = mFs*0.5*0.95;
    mMinFc = 10.0;
    
    feedbackL = 0;
    feedbackR = 0;

}

void EjmfinalphaserAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool EjmfinalphaserAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

// initialize needed variables
void EjmfinalphaserAudioProcessor::calcAlgorithmParams(){
    
    // wet dry mix
    mDryGain = 1 - (mWetDry->get() / 100);
    mWetGain = (mWetDry->get() / 100);
    
    // set the rate of the LFO - create "2 on 3" effect
    mLFO1.setFreq(mLFORate->get(), mFs);
    mLFO2.setFreq((mLFORate->get()) * 2/3.0, mFs);
    mLFO3.setFreq((mLFORate->get()) * 1/2, mFs);
    
}

void EjmfinalphaserAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    // Update the parameter values
    calcAlgorithmParams();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    auto* channelDataLeft = buffer.getWritePointer(0);
    auto* channelDataRight = buffer.getWritePointer(1);

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
    float outL, outR, tempL, tempR, lfoOut1, lfoOut2, lfoOut3;
    float coeffs1[5], coeffs2[5], coeffs3[5];
    
    for (int samp = 0; samp < buffer.getNumSamples(); samp++)
    {
        
        // LFO stuff, coefficients
        lfoOut1 = mLFO1.tick();
        lfoOut2 = mLFO2.tick();
        lfoOut3 = mLFO3.tick();
        
        
        Mu45FilterCalc::calcCoeffsAPF(coeffs1, ((lfoOut1+1)*mLFODepth->get()*500)+200, 0.7, mFs);
        mAPF1R.setCoefficients(coeffs1[0], coeffs1[1], coeffs1[2], coeffs1[3], coeffs1[4]);
        mAPF1L.setCoefficients(coeffs1[0], coeffs1[1], coeffs1[2], coeffs1[3], coeffs1[4]);
        
        Mu45FilterCalc::calcCoeffsAPF(coeffs2, ((lfoOut2+1)*mLFODepth->get()*500)+2000, 0.7, mFs);
        mAPF2R.setCoefficients(coeffs2[0], coeffs2[1], coeffs2[2], coeffs2[3], coeffs2[4]);
        mAPF2L.setCoefficients(coeffs2[0], coeffs2[1], coeffs2[2], coeffs2[3], coeffs2[4]);
        
        Mu45FilterCalc::calcCoeffsAPF(coeffs3, ((lfoOut3+1)*mLFODepth->get()*500)+1000, 0.7, mFs);
        mAPF3R.setCoefficients(coeffs3[0], coeffs3[1], coeffs3[2], coeffs3[3], coeffs3[4]);
        mAPF3L.setCoefficients(coeffs3[0], coeffs3[1], coeffs3[2], coeffs3[3], coeffs3[4]);
        
        
        
        // ----------------------------------------------------------- //
        
        // Actual signal processing
        
        // apply dry gain
        outL = channelDataLeft[samp] * mDryGain;
        outR = channelDataRight[samp] * mDryGain;
        
        // do wet operations
        tempL = channelDataLeft[samp];
        tempR = channelDataRight[samp];
        
        // all pass filter 1
        tempL = mAPF1L.tick(tempL + feedbackL * (mFeedback->get() * 0.01));
        tempR = mAPF1R.tick(tempR + feedbackR * (mFeedback->get() * 0.01));
        
        // all pass filter 2
        tempL = mAPF2L.tick(tempL);
        tempR = mAPF2R.tick(tempR);

        // all pass filter 3
        tempL = mAPF3L.tick(tempL);
        tempR = mAPF3R.tick(tempR);
        
        // feedback signal = output of all pass filter 3
        feedbackL = tempL;
        feedbackR = tempR;
        
        // mix wet and dry into the output
        outL += mWetGain * tempL;
        outR += mWetGain * tempR;
        
        // send out
        channelDataLeft[samp] = outL;
        channelDataRight[samp] = outR;
        
    }
}

//==============================================================================
bool EjmfinalphaserAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* EjmfinalphaserAudioProcessor::createEditor()
{
    return new EjmfinalphaserAudioProcessorEditor (*this);
}

//==============================================================================
void EjmfinalphaserAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void EjmfinalphaserAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new EjmfinalphaserAudioProcessor();
}

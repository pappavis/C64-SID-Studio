// Vliegbasis71 SID Studio 64 | v0.3.5 | ChatID C5A9E2D7
#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "SidEngine.hpp"
#include "StepSequencer.hpp"
#include "Arpeggiator.hpp"
#include <atomic>
class SIDProcessor final : public juce::AudioProcessor {
public:
    SIDProcessor();
    const juce::String getName() const override { return "SID Studio 64"; }
    void prepareToPlay(double sampleRate, int maximumExpectedSamplesPerBlock) override;
    void releaseResources() override {}
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    bool isBusesLayoutSupported(const BusesLayout&) const override;
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.2; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return "Begin"; }
    void changeProgramName(int, const juce::String&) override {}
    void getStateInformation(juce::MemoryBlock&) override;
    void setStateInformation(const void*, int) override;
    juce::AudioProcessorValueTreeState params;
    std::atomic<double> hostTempo{120.0};
    std::atomic<bool> hostPlaying{false};
    std::atomic<int> activeStep{-1};
private:
    sid64::SidEngine engine;
    sid64::StepSequencer sequencer;
    sid64::Arpeggiator arpeggiator;
    int generatedNote=-1;
    int previousPlayMode=-1;
    bool previousHold=false;
    double freePpq=0.0;
    std::vector<float> scratchL, scratchR;
    int lastMode = -1, lastWave = -1;
    void applyParameters();
    void renderChunk(juce::AudioBuffer<float>& audio, int start, int count);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SIDProcessor)
};

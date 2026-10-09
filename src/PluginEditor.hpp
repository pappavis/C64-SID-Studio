// Vliegbasis71 SID Studio 64 | v0.3.2 | ChatID E2C7A94F
#pragma once
#include <juce_gui_extra/juce_gui_extra.h>
#include "PluginProcessor.hpp"
#include <array>
class SIDEditor final : public juce::AudioProcessorEditor, private juce::Timer {
public:
    explicit SIDEditor(SIDProcessor&);
    void paint(juce::Graphics&) override;
    void resized() override;
private:
    SIDProcessor& sidProcessor;
    juce::ComboBox mode, wave;
    juce::Label modeLabel, waveLabel;
    std::array<juce::Slider,4> envSliders;
    std::array<juce::Slider,3> chipSliders;
    std::array<juce::Label,3> chipLabels;
    std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>,3> chipAttachments;
    std::array<juce::ToggleButton,3> chipSwitches;
    std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>,3> chipSwitchAttachments;
    std::array<juce::Label,4> envLabels;
    std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>,4> envAttachments;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> modeAttachment, waveAttachment;
    std::array<juce::TextButton,3> languageButtons;
    juce::TextButton soundTab, sequenceTab, effectsTab;
    juce::ToggleButton seqEnabled;
    juce::ComboBox playMode, arpDirection;
    juce::ToggleButton holdButton;
    juce::Label playModeLabel, arpDirectionLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> playModeAttachment, arpDirectionAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> holdAttachment;
    juce::ComboBox seqDivision, seqLength;
    juce::Slider seqNote;
    juce::Label tempoLabel, seqHelp;
    std::array<juce::ToggleButton,32> steps;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> seqEnabledAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> seqDivisionAttachment,seqLengthAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> seqNoteAttachment;
    std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>,32> stepAttachments;
    void timerCallback() override;
    void showPage();
    int language=0, page=0;
    void updateLanguage();
    void drawPanel(juce::Graphics&,juce::Rectangle<int>,const juce::String&);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SIDEditor)
};

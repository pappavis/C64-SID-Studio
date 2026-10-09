// Vliegbasis71 SID Studio 64 | v0.3.5 | ChatID C5A9E2D7
#include "PluginProcessor.hpp"
#include "PluginEditor.hpp"
#include <algorithm>
namespace {
juce::AudioProcessorValueTreeState::ParameterLayout makeLayout() {
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> p;
    p.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{"modus",1}, "Modus", juce::StringArray{"Oorspronklik", "Uitgebrei"}, 0));
    p.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{"golf",1}, "Golfvorm", juce::StringArray{"Driehoek", "Saagtand", "Puls", "Ruis"}, 1));
    p.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"aanval",1}, "Aanval", juce::NormalisableRange<float>(0.001f,2.f,0.001f,0.45f),0.01f));
    p.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"verval",1}, "Verval", juce::NormalisableRange<float>(0.005f,3.f,0.001f,0.45f),0.1f));
    p.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"aanhou",1}, "Aanhou", juce::NormalisableRange<float>(0.f,1.f,0.001f),0.7f));
    p.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"vrylating",1}, "Vrylating", juce::NormalisableRange<float>(0.005f,4.f,0.001f,0.45f),0.2f));
    p.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"pulsbreedte",1}, "Pulsbreedte", juce::NormalisableRange<float>(0.02f,0.98f,0.001f),0.5f));
    p.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"filtergrens",1}, "Filtergrens", juce::NormalisableRange<float>(30.f,12000.f,1.f,0.35f),6000.f));
    p.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"resonansie",1}, "Resonansie", juce::NormalisableRange<float>(0.f,0.95f,0.001f),0.2f));
    p.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{"filteraan",1}, "Filter aan",false));
    p.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{"ring",1}, "Ringmodulasie",false));
    p.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{"sink",1}, "Sinkronisasie",false));
    p.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{"reeksaan",1},"Reeks aan",false));
    p.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{"reeksdeling",1},"Reeksdeling",juce::StringArray{"Kwart","Agste","Sestiende","Twee-en-dertigste"},2));
    p.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{"reekslengte",1},"Reekslengte",juce::StringArray{"16","32"},0));
    p.push_back(std::make_unique<juce::AudioParameterInt>(juce::ParameterID{"reeksnoot",1},"Reeksnoot",36,84,48));
    for(int i=0;i<32;++i) p.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{"stap"+juce::String(i),1},"Stap "+juce::String(i+1),i%4==0));
    p.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{"speelwyse",1},"Speelwyse",juce::StringArray{"Regstreeks","Stappe","Arpeggio"},0));
    p.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{"arprigting",1},"Arpeggiorigting",juce::StringArray{"Op","Af","Op en af","Willekeurig","Speelvolgorde"},0));
    p.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{"houvas",1},"Hou vas",false));
    return {p.begin(),p.end()};
}
}
SIDProcessor::SIDProcessor() : AudioProcessor(BusesProperties().withOutput("Uitvoer", juce::AudioChannelSet::stereo(), true)), params(*this, nullptr, "Instellings", makeLayout()), engine(44100.f) {}
void SIDProcessor::prepareToPlay(double rate, int maxBlock) {
    engine = sid64::SidEngine(static_cast<float>(rate));
    lastMode = lastWave = -1;
    scratchL.resize(static_cast<size_t>(std::max(1,maxBlock)));
    scratchR.resize(static_cast<size_t>(std::max(1,maxBlock)));
    applyParameters();
    generatedNote=-1; previousPlayMode=-1; previousHold=false; arpeggiator.clear(); freePpq=0;
}
bool SIDProcessor::isBusesLayoutSupported(const BusesLayout& layout) const {
    return layout.getMainOutputChannelSet() == juce::AudioChannelSet::stereo() && layout.getMainInputChannelSet().isDisabled();
}
void SIDProcessor::applyParameters() {
    const int mode = static_cast<int>(params.getRawParameterValue("modus")->load());
    const int wave = static_cast<int>(params.getRawParameterValue("golf")->load());
    if (mode != lastMode) { engine.setMode(mode == 0 ? sid64::Mode::Authentic : sid64::Mode::Deluxe); lastMode = mode; }
    if (wave != lastWave) { engine.setWave(static_cast<sid64::Wave>(std::clamp(wave,0,3))); lastWave = wave; }
    sid64::Envelope e;
    e.attack=params.getRawParameterValue("aanval")->load();
    e.decay=params.getRawParameterValue("verval")->load();
    e.sustain=params.getRawParameterValue("aanhou")->load();
    e.release=params.getRawParameterValue("vrylating")->load();
    engine.setEnvelope(e);
    engine.setPulseWidth(params.getRawParameterValue("pulsbreedte")->load());
    engine.setFilter(params.getRawParameterValue("filtergrens")->load(),
                     params.getRawParameterValue("resonansie")->load(),
                     params.getRawParameterValue("filteraan")->load()>0.5f);
    engine.setRingMod(params.getRawParameterValue("ring")->load()>0.5f);
    engine.setHardSync(params.getRawParameterValue("sink")->load()>0.5f);
}
void SIDProcessor::renderChunk(juce::AudioBuffer<float>& audio, int start, int count) {
    while(count > 0) {
        const int n = std::min(count, static_cast<int>(scratchL.size()));
        if(n <= 0) return;
        engine.render(scratchL.data(), scratchR.data(), static_cast<size_t>(n));
        audio.copyFrom(0, start, scratchL.data(), n);
        audio.copyFrom(1, start, scratchR.data(), n);
        start += n; count -= n;
    }
}
void SIDProcessor::processBlock(juce::AudioBuffer<float>& audio, juce::MidiBuffer& midi) {
    juce::ScopedNoDenormals noDenormals;
    audio.clear();
    applyParameters();
    const int frames=audio.getNumSamples();
    double bpm=120.0, ppq=freePpq;
    bool playing=(getPlayHead()==nullptr), hasHostPpq=false;
    if (auto* head=getPlayHead()) {
        if (auto position=head->getPosition()) {
            if (auto hostBpm=position->getBpm()) bpm=*hostBpm;
            playing=position->getIsPlaying();
            if (auto hostPpq=position->getPpqPosition()) {ppq=*hostPpq;hasHostPpq=true;}
        }
    }
    bpm=std::clamp(bpm,20.0,400.0);
    hostTempo.store(bpm,std::memory_order_relaxed);
    hostPlaying.store(playing,std::memory_order_relaxed);
    const int playMode=static_cast<int>(params.getRawParameterValue("speelwyse")->load());
    const bool legacySeq=params.getRawParameterValue("reeksaan")->load()>0.5f;
    const bool stepMode=(playMode==1)||(playMode==0 && legacySeq);
    const bool arpMode=playMode==2;
    const bool hold=params.getRawParameterValue("houvas")->load()>0.5f;
    // Keep physical keys separate from latched notes. Default is hold OFF.
    if (playMode != previousPlayMode) {
        engine.allNotesOff(); generatedNote=-1; arpeggiator.clear();
        previousPlayMode=playMode;
    }
    if (hold != previousHold) {
        arpeggiator.setHold(hold);
        if (arpMode && arpeggiator.count()==0 && generatedNote>=0) {
            engine.noteOff(generatedNote); generatedNote=-1;
        }
        previousHold=hold;
    }
    // Generated modes also work when Logic transport is stopped: use an internal
    // musical clock. Host PPQ remains authoritative while Logic is playing.
    // This makes holding a chord immediately audible in Arpeggio mode.
    const double clockPpq = (playing && hasHostPpq) ? ppq : freePpq;
    if ((!stepMode && !arpMode) && generatedNote >= 0) {
        engine.noteOff(generatedNote); generatedNote = -1;
    }
    std::array<bool,32> pattern{};
    for(int i=0;i<32;++i) pattern[static_cast<size_t>(i)]=params.getRawParameterValue("stap"+juce::String(i))->load()>0.5f;
    sid64::StepEvent events[sid64::StepSequencer::maxEvents]{};
    const int division=static_cast<int>(params.getRawParameterValue("reeksdeling")->load());
    const int length=params.getRawParameterValue("reekslengte")->load()>0.5f?32:16;
    // For arp every grid position triggers a note; steps mode respects its gate pattern.
    if(arpMode) pattern.fill(true);
    const int count=(stepMode||arpMode) ? sequencer.schedule(clockPpq,bpm,getSampleRate(),frames,division,length,
                     pattern,events,sid64::StepSequencer::maxEvents):0;
    activeStep.store(count>0?events[count-1].step:-1,std::memory_order_relaxed);
    int cursor=0, index=0;
    auto it=midi.begin();
    while(it!=midi.end() || index<count) {
        const int hostPos=it!=midi.end()?std::clamp((*it).samplePosition,0,frames):frames+1;
        const int seqPos=index<count?events[index].sample:frames+1;
        const int pos=std::min(hostPos,seqPos);
        if(pos>cursor) renderChunk(audio,cursor,pos-cursor);
        cursor=pos;
        // MIDI at the same sample is processed BEFORE the generated step, so a chord
        // entered at PPQ 0 can trigger its first arpeggio note at sample zero.
        if(hostPos<=seqPos) {
            const auto msg=(*it).getMessage();
            if(msg.isNoteOn()) {
                if(arpMode) arpeggiator.noteOn(msg.getNoteNumber(),hold);
                else if(!stepMode) engine.noteOn(msg.getNoteNumber());
            } else if(msg.isNoteOff()) {
                if(arpMode) { arpeggiator.noteOff(msg.getNoteNumber(),hold);
                    if(arpeggiator.count()==0 && generatedNote>=0){engine.noteOff(generatedNote);generatedNote=-1;} }
                else if(!stepMode) engine.noteOff(msg.getNoteNumber());
            } else if(msg.isAllNotesOff()||msg.isAllSoundOff()) {
                engine.allNotesOff();arpeggiator.clear();generatedNote=-1;
            }
            ++it;
        } else {
            if(generatedNote>=0){engine.noteOff(generatedNote);generatedNote=-1;}
            if(events[index].on){
                const int note=arpMode?arpeggiator.next(static_cast<int>(params.getRawParameterValue("arprigting")->load()))
                                      :static_cast<int>(params.getRawParameterValue("reeksnoot")->load());
                if(note>=0){engine.noteOn(note);generatedNote=note;}
            }
            ++index;
        }
    }
    if(cursor<frames) renderChunk(audio,cursor,frames-cursor);
    if (getSampleRate() > 0) {
        const double advance = frames * bpm / (60.0 * getSampleRate());
        freePpq = clockPpq + advance;
    }
}
void SIDProcessor::getStateInformation(juce::MemoryBlock& data) {
    auto state = params.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, data);
}
void SIDProcessor::setStateInformation(const void* data, int bytes) {
    auto xml = getXmlFromBinary(data, bytes);
    if(xml && xml->hasTagName(params.state.getType())) params.replaceState(juce::ValueTree::fromXml(*xml));
}
juce::AudioProcessorEditor* SIDProcessor::createEditor() { return new SIDEditor(*this); }
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new SIDProcessor(); }

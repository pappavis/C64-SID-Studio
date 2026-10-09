// Vliegbasis71 SID Studio 64 | v0.1.0 | ChatID B6E2A94C
#include "SidEngine.hpp"
#include <algorithm>
#include <cmath>

namespace sid64 {
namespace {
constexpr float pi=3.14159265358979323846f;
constexpr float sidClock=985248.f; // PAL C64 nominal clock, 6581-inspired mode
float frequency(int note) noexcept { return 440.f*std::pow(2.f,(note-69)/12.f); }
float timeStep(float seconds,float rate) noexcept { return seconds<=0.f ? 1.f : 1.f/(seconds*rate); }
}
SidEngine::SidEngine(float sampleRate):sampleRate_(sampleRate>1000.f?sampleRate:44100.f) {}
void SidEngine::setMode(Mode mode) noexcept {
    if(mode_!=mode) { allNotesOff(); mode_=mode; nextVoice_=0; }
}
void SidEngine::setEnvelope(Envelope env) noexcept {
    env.attack=std::max(0.f,env.attack);
    env.decay=std::max(0.f,env.decay);
    env.sustain=std::clamp(env.sustain,0.f,1.f);
    env.release=std::max(0.f,env.release);
    env_=env;
}
void SidEngine::setPulseWidth(float width) noexcept { pulseWidth_=std::clamp(width,1.f/4096.f,4095.f/4096.f); }
void SidEngine::setFilter(float cutoffHz, float resonance, bool enabled) noexcept {
    cutoff_=std::clamp(cutoffHz,30.f,12000.f);
    resonance_=std::clamp(resonance,0.f,0.95f);
    filterEnabled_=enabled;
}
float SidEngine::filterSample(float input) noexcept {
    if(!filterEnabled_) return input;
    // Stable 2x-oversampled state-variable low-pass: artistic SID-style, NOT 6581 circuit model.
    const float f=2.f*std::sin(pi*std::min(cutoff_,sampleRate_*0.20f)/(sampleRate_*2.f));
    const float damping=1.8f-1.65f*resonance_;
    for(int i=0;i<2;++i) {
        filterLow_+=f*filterBand_;
        const float high=input-filterLow_-damping*filterBand_;
        filterBand_+=f*high;
    }
    if(!std::isfinite(filterLow_) || !std::isfinite(filterBand_)) filterLow_=filterBand_=0.f;
    return std::clamp(filterLow_,-8.f,8.f);
}
void SidEngine::noteOn(int midiNote) noexcept {
    if(midiNote<0 || midiNote>127) return;
    // Prefer an unused voice; if full, steal in round-robin order.
    const auto limit=voiceLimit();
    size_t index=limit;
    for(size_t i=0;i<limit;++i) {
        const size_t candidate=(nextVoice_+i)%limit;
        if(voices_[candidate].stage==Voice::Stage::Off) { index=candidate; break; }
    }
    if(index==limit) index=nextVoice_%limit;
    auto& v=voices_[index];
    v=Voice{}; v.note=midiNote; v.gate=true; v.wave=wave_; v.env=env_;
    v.stage=Voice::Stage::Attack;
    v.pulseWidth=pulseWidth_;
    v.noiseState=0x7ffff8u;
    nextVoice_=static_cast<uint32_t>((index+1)%limit);
}
void SidEngine::noteOff(int midiNote) noexcept {
    for(size_t i=0;i<voiceLimit();++i) {
        auto& v=voices_[i];
        if(v.note==midiNote && v.gate) { v.gate=false; v.stage=Voice::Stage::Release; }
    }
}
void SidEngine::allNotesOff() noexcept { for(auto& v:voices_) v=Voice{}; }
size_t SidEngine::activeVoices() const noexcept {
    size_t count=0;
    for(size_t i=0;i<voiceLimit();++i) if(voices_[i].stage!=Voice::Stage::Off) ++count;
    return count;
}
float SidEngine::sampleVoice(Voice& v) noexcept {
    if(v.stage==Voice::Stage::Off) return 0.f;
    switch(v.stage) {
        case Voice::Stage::Attack:
            v.level=std::min(1.f,v.level+timeStep(v.env.attack,sampleRate_));
            if(v.level>=1.f) v.stage=Voice::Stage::Decay;
            break;
        case Voice::Stage::Decay:
            v.level=std::max(v.env.sustain,v.level-timeStep(v.env.decay,sampleRate_)*(1.f-v.env.sustain));
            if(v.level<=v.env.sustain) v.stage=Voice::Stage::Sustain;
            break;
        case Voice::Stage::Sustain: v.level=v.env.sustain; break;
        case Voice::Stage::Release:
            v.level=std::max(0.f,v.level-timeStep(v.env.release,sampleRate_));
            if(v.level<=0.f) { v.stage=Voice::Stage::Off; v.note=-1; return 0.f; }
            break;
        case Voice::Stage::Off: return 0.f;
    }
    const float phase=v.phase;
    // Quantise oscillator pitch to the SID's 16-bit frequency register in authentic mode.
    const float hz=frequency(v.note);
    const float increment=mode_==Mode::Authentic
        ? (std::round(hz*16777216.f/sidClock)*sidClock/16777216.f)/sampleRate_
        : hz/sampleRate_;
    const float previousMod=v.modPhase;
    v.modPhase+=increment*1.003f;
    v.modPhase-=std::floor(v.modPhase);
    if(hardSync_ && v.modPhase<previousMod) v.phase=0.f;
    float sample=0.f;
    switch(v.wave) {
        case Wave::Triangle: sample=4.f*std::fabs(phase-0.5f)-1.f; if(ringMod_ && v.modPhase>=0.5f) sample=-sample; break;
        case Wave::Saw: sample=2.f*phase-1.f; break;
        case Wave::Pulse: sample=phase<v.pulseWidth?1.f:-1.f; break;
        case Wave::Noise:
            // 23-bit feedback shift register, clocked at a reduced oscillator cadence.
            if(phase<increment) {
                const uint32_t bit=((v.noiseState>>22)^(v.noiseState>>17))&1u;
                v.noiseState=((v.noiseState<<1)|bit)&0x7fffffu;
            }
            sample=(static_cast<float>((v.noiseState>>7)&255u)/127.5f)-1.f;
            break;
    }
    v.phase+=increment;
    v.phase-=std::floor(v.phase);
    return sample*v.level;
}
void SidEngine::render(float* left,float* right,size_t frames) noexcept {
    if(!left || !right) return;
    for(size_t n=0;n<frames;++n) {
        float sum=0.f;
        for(size_t i=0;i<voiceLimit();++i) sum+=sampleVoice(voices_[i]);
        const float output=std::tanh(filterSample(sum*0.25f));
        left[n]=output; right[n]=output;
    }
}
}

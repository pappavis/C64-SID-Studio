// Vliegbasis71 SID Studio 64 | v0.1.0 | ChatID B6E2A94C
#include "SidEngine.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>
using namespace sid64;
int main() {
    SidEngine synth(48000.f);
    assert(synth.mode()==Mode::Authentic && synth.voiceLimit()==3);
    synth.noteOn(60); synth.noteOn(64); synth.noteOn(67); synth.noteOn(72);
    assert(synth.activeVoices()==3);
    std::vector<float> left(4800),right(4800);
    synth.render(left.data(),right.data(),left.size());
    float energy=0.f;
    for(size_t i=0;i<left.size();++i) {
        assert(std::isfinite(left[i]) && left[i]==right[i]);
        assert(std::fabs(left[i])<=1.f);
        energy+=std::fabs(left[i]);
    }
    assert(energy>1.f);
    synth.setMode(Mode::Deluxe);
    assert(synth.voiceLimit()==16 && synth.activeVoices()==0);
    for(int n=48;n<64;++n) synth.noteOn(n);
    assert(synth.activeVoices()==16);
    synth.noteOn(80);
    assert(synth.activeVoices()==16);
    synth.allNotesOff();
    assert(synth.activeVoices()==0);
    synth.setEnvelope({0.f,0.f,1.f,0.f});
    synth.setWave(Wave::Triangle);
    synth.noteOn(69);
    synth.render(left.data(),right.data(),left.size());
    synth.noteOff(69);
    synth.render(left.data(),right.data(),left.size());
    assert(synth.activeVoices()==0);
    std::cout<<"GREEN: 12 checks passed (modes, polyphony, audio, release, safety)\n";
}

// Additional 3B sound-engine regression tests are in test_sid_character.cpp.

// Vliegbasis71 SID Studio 64 | v0.3.2 | ChatID E2C7A94F
#include "SidEngine.hpp"
#include <cassert>
#include <cmath>
#include <vector>
#include <algorithm>
int main() {
    sid64::SidEngine e(44100.f);
    assert(e.voiceLimit()==3);
    e.setMode(sid64::Mode::Deluxe);
    assert(e.voiceLimit()==16);
    e.setMode(sid64::Mode::Authentic);
    e.setWave(sid64::Wave::Pulse);
    e.setPulseWidth(0.12f);
    e.setEnvelope({0.001f,0.01f,1.f,0.02f});
    e.setFilter(1100.f,0.6f,true);
    e.setRingMod(true);
    e.setHardSync(true);
    e.noteOn(60);
    std::vector<float> l(2048),r(2048);
    e.render(l.data(),r.data(),l.size());
    float peak=0.f;
    for(size_t i=0;i<l.size();++i) { assert(std::isfinite(l[i]));assert(l[i]==r[i]);peak=std::max(peak,std::fabs(l[i])); }
    assert(peak>0.0001f && peak<=1.f);
    e.noteOff(60);
    for(int i=0;i<10;++i)e.render(l.data(),r.data(),l.size());
    assert(e.activeVoices()==0);
    e.setFilter(12000.f,0.95f,true);
    e.setWave(sid64::Wave::Noise);
    e.noteOn(72);
    e.render(l.data(),r.data(),l.size());
    for(float x:l) assert(std::isfinite(x));
}

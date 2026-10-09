// Vliegbasis71 SID Studio 64 | v0.1.0 | ChatID B6E2A94C
#pragma once
#include <array>
#include <cstdint>
#include <cstddef>

namespace sid64 {
enum class Wave : uint8_t { Triangle, Saw, Pulse, Noise };
enum class Mode : uint8_t { Authentic, Deluxe };
struct Envelope { float attack=0.01f, decay=0.1f, sustain=0.7f, release=0.2f; };
struct Voice {
    bool gate=false;
    int note=-1;
    Wave wave=Wave::Saw;
    Envelope env{};
    float phase=0.f, level=0.f, pulseWidth=0.5f;
    uint32_t noiseState=0x7ffff8u;
    float modPhase=0.f;
    enum class Stage { Off, Attack, Decay, Sustain, Release } stage=Stage::Off;
};
class SidEngine {
public:
    explicit SidEngine(float sampleRate=44100.f);
    void setMode(Mode mode) noexcept;
    Mode mode() const noexcept { return mode_; }
    void setWave(Wave wave) noexcept { wave_=wave; }
    void setEnvelope(Envelope env) noexcept;
    void setPulseWidth(float width) noexcept;
    void setFilter(float cutoffHz, float resonance, bool enabled) noexcept;
    void setRingMod(bool enabled) noexcept { ringMod_=enabled; }
    void setHardSync(bool enabled) noexcept { hardSync_=enabled; }
    void noteOn(int midiNote) noexcept;
    void noteOff(int midiNote) noexcept;
    void allNotesOff() noexcept;
    void render(float* left, float* right, size_t frames) noexcept;
    size_t activeVoices() const noexcept;
    size_t voiceLimit() const noexcept { return mode_==Mode::Authentic ? 3u : 16u; }
private:
    float sampleRate_;
    Mode mode_=Mode::Authentic;
    Wave wave_=Wave::Saw;
    Envelope env_{};
    std::array<Voice,16> voices_{};
    uint32_t nextVoice_=0;
    float pulseWidth_=0.5f, cutoff_=11000.f, resonance_=0.2f;
    bool filterEnabled_=false, ringMod_=false, hardSync_=false;
    float filterLow_=0.f, filterBand_=0.f;
    float filterSample(float input) noexcept;
    float sampleVoice(Voice& voice) noexcept;
};
}

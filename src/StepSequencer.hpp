// Vliegbasis71 SID Studio 64 | v0.3.2 | ChatID E2C7A94F
#pragma once
#include <array>
#include <cstdint>
namespace sid64 {
struct StepEvent { int sample=0; int step=0; bool on=false; };
class StepSequencer {
public:
 static constexpr int maxEvents=256;
 // ppq is quarter notes, 1/16 = 0.25 quarter notes.
 int schedule(double ppq, double bpm, double sampleRate, int frames, int division,
              int length, const std::array<bool,32>& pattern, StepEvent* out, int capacity) const noexcept;
};
}

// Vliegbasis71 | v0.3.5 | ChatID C5A9E2D7
#pragma once
#include <array>
#include <cstdint>
namespace sid64 {
class Arpeggiator {
public:
 void noteOn(int note, bool hold=false) noexcept;
 void noteOff(int note, bool hold) noexcept;
 void setHold(bool hold) noexcept;
 void clear() noexcept;
 int next(int direction) noexcept;
 int count() const noexcept { return size_; }
 int physicalCount() const noexcept;
private:
 std::array<bool,128> physical_{};
 std::array<bool,128> latched_{};
 std::array<int,128> order_{};
 int size_=0, position_=0, travel_=1;
 uint32_t seed_=0x12345678u;
 void remove(int note) noexcept;
};
}

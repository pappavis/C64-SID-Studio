// Vliegbasis71 SID Studio 64 | v0.3.2 | ChatID E2C7A94F
#include "StepSequencer.hpp"
#include <algorithm>
#include <cmath>
namespace sid64 {
int StepSequencer::schedule(double ppq,double bpm,double sampleRate,int frames,int division,int length,
                            const std::array<bool,32>& pattern,StepEvent* out,int capacity) const noexcept {
 if (!out || frames<=0 || bpm<=0 || sampleRate<=0 || !std::isfinite(ppq) || !std::isfinite(bpm)) return 0;
 division=std::clamp(division,0,3); length=std::clamp(length,1,32);
 const double stepBeats=1.0/static_cast<double>(1 << division); // 1/4,1/8,1/16,1/32
 const double beatsPerSample=bpm/(60.0*sampleRate);
 const double end=ppq+frames*beatsPerSample;
 auto tick=static_cast<std::int64_t>(std::ceil((ppq-1e-10)/stepBeats));
 int count=0;
 while(count<capacity && tick*stepBeats<end-1e-10) {
   const double beat=tick*stepBeats;
   const int sample=std::clamp(static_cast<int>(std::round((beat-ppq)/beatsPerSample)),0,frames-1);
   const int step=static_cast<int>((tick%length+length)%length);
   out[count++]={sample,step,pattern[static_cast<size_t>(step)]};
   ++tick;
 }
 return count;
}
}

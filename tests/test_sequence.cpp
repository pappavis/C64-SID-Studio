// Vliegbasis71 SID Studio 64 | v0.3.2 | ChatID E2C7A94F
#include "StepSequencer.hpp"
#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
int main(){
 sid64::StepSequencer s;std::array<bool,32> p{};p[0]=true;p[4]=true;
 sid64::StepEvent e[256]{};
 // 88 BPM: 1/16 step = 60/88/4 seconds = 0.17045 seconds
 auto n=s.schedule(0,88,48000,48000,2,16,p,e,256);
 assert(n==6);assert(e[0].sample==0 && e[0].on && e[0].step==0);
 assert(std::abs(e[1].sample-8182)<=1 && e[1].step==1);
 assert(e[4].on && e[4].step==4);
 // Start from the middle of a measure: first boundary at quarter-note 0.5.
 n=s.schedule(0.4,120,48000,6000,2,16,p,e,256);
 assert(n==1 && e[0].step==2 && e[0].sample==2400);
 // At 120 BPM 1/16 interval is 6000 samples; a tempo change changes spacing.
 n=s.schedule(0,120,48000,24000,2,16,p,e,256);
 assert(n==4 && e[1].sample==6000);
 // 32-step pattern, negative timeline, non-playing host behavior handled by caller.
 p[31]=true;n=s.schedule(7.75,120,48000,1000,2,32,p,e,256);
 assert(n==1 && e[0].step==31 && e[0].on);
 std::cout<<"sequence timing OK\n";
}

// Vliegbasis71 | v0.3.5 | ChatID C5A9E2D7
#include "Arpeggiator.hpp"
#include <cassert>
int main(){
 sid64::Arpeggiator a;
 assert(a.count()==0);
 // Default: releasing last key must silence arp, never latch.
 a.noteOn(60);a.noteOn(64);a.noteOn(67);
 assert(a.count()==3 && a.physicalCount()==3);
 assert(a.next(0)==60);assert(a.next(0)==64);assert(a.next(0)==67);
 a.noteOff(60,false);assert(a.count()==2 && a.physicalCount()==2);
 a.noteOff(64,false);a.noteOff(67,false);
 assert(a.count()==0 && a.next(0)==-1);
 // Hold enabled: released notes remain until hold disabled.
 a.noteOn(60,true);a.noteOn(64,true);a.noteOn(67,true);
 a.noteOff(60,true);a.noteOff(64,true);a.noteOff(67,true);
 assert(a.count()==3 && a.physicalCount()==0);
 a.setHold(false);assert(a.count()==0 && a.next(0)==-1);
 // Switching hold off retains physically pressed keys only.
 a.noteOn(60,true);a.noteOn(64,true);a.noteOff(60,true);
 a.setHold(false);assert(a.count()==1 && a.next(0)==64);
 a.noteOff(64,false);assert(a.count()==0);
 // New chord replaces previously latched chord.
 a.noteOn(60,true);a.noteOn(64,true);a.noteOff(60,true);a.noteOff(64,true);
 a.noteOn(67,true);assert(a.count()==1 && a.next(0)==67);
 a.noteOff(67,true);a.clear();assert(a.count()==0);
 // Direction modes retained.
 a.noteOn(67);a.noteOn(60);a.noteOn(64);
 assert(a.next(1)==67);assert(a.next(1)==64);assert(a.next(1)==60);
 a.clear();a.noteOn(60);a.noteOn(64);a.noteOn(67);
 assert(a.next(2)==60);assert(a.next(2)==64);assert(a.next(2)==67);
 a.clear();a.noteOn(67);a.noteOn(60);a.noteOn(64);
 assert(a.next(4)==67);assert(a.next(4)==60);assert(a.next(4)==64);
}

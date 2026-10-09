// Vliegbasis71 | v0.3.5 | ChatID C5A9E2D7
#include "Arpeggiator.hpp"
#include <algorithm>
#include <cstddef>
namespace sid64 {
void Arpeggiator::remove(int note) noexcept {
 const auto n=static_cast<size_t>(note);
 latched_[n]=false;
 for(int i=0;i<size_;++i) {
  if(order_[static_cast<size_t>(i)]!=note) continue;
  for(int j=i;j<size_-1;++j) order_[static_cast<size_t>(j)]=order_[static_cast<size_t>(j+1)];
  --size_; position_=0; travel_=1; break;
 }
}
void Arpeggiator::noteOn(int note,bool hold) noexcept {
 if(note<0||note>127) return;
 const auto n=static_cast<size_t>(note);
 if(physical_[n]) return;
 // New chord after releasing everything replaces the previous latched chord.
 if(hold && physicalCount()==0) {
  for(int i=size_-1;i>=0;--i) {
   const int old=order_[static_cast<size_t>(i)];
   if(!physical_[static_cast<size_t>(old)]) remove(old);
  }
 }
 physical_[n]=true;
 if(latched_[n]) return;
 latched_[n]=true;order_[static_cast<size_t>(size_++)]=note;
 position_=0;travel_=1;
}
void Arpeggiator::noteOff(int note,bool hold) noexcept {
 if(note<0||note>127) return;
 const auto n=static_cast<size_t>(note);
 physical_[n]=false;
 if(!hold) remove(note);
}
void Arpeggiator::setHold(bool hold) noexcept {
 if(hold) return;
 for(int i=size_-1;i>=0;--i) {
  const int n=order_[static_cast<size_t>(i)];
  if(!physical_[static_cast<size_t>(n)]) remove(n);
 }
}
int Arpeggiator::physicalCount() const noexcept {
 return static_cast<int>(std::count(physical_.begin(),physical_.end(),true));
}
void Arpeggiator::clear() noexcept {
 physical_.fill(false);latched_.fill(false);size_=0;position_=0;travel_=1;
}
int Arpeggiator::next(int direction) noexcept {
 if(size_==0)return -1;
 std::array<int,128> notes=order_;
 if(direction!=4)std::sort(notes.begin(),notes.begin()+size_);
 int index=0;
 if(direction==3){seed_^=seed_<<13;seed_^=seed_>>17;seed_^=seed_<<5;index=static_cast<int>(seed_%static_cast<uint32_t>(size_));}
 else if(direction==1){index=size_-1-position_;position_=(position_+1)%size_;}
 else if(direction==2){index=position_;if(size_>1){position_+=travel_;if(position_>=size_-1){position_=size_-1;travel_=-1;}else if(position_<=0){position_=0;travel_=1;}}}
 else {index=position_;position_=(position_+1)%size_;}
 return notes[static_cast<size_t>(index)];
}
}

#include "timer.h"

void Timer::update(unsigned int elapsed) {
  value_ += elapsed;
  overflow_ = value_ >= interval_;
  if (overflow_) value_ -= interval_;
}

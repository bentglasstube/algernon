#include "timer.h"

Timer::Timer(unsigned int interval) : value_(0), interval_(interval) {}

void Timer::update(unsigned int elapsed) {
  value_ += elapsed;
  overflow_ = value_ >= interval_;
  if (overflow_) value_ -= interval_;
}

bool Timer::fired() const {
  return overflow_;
}

unsigned int Timer::value() const {
  return value_;
}

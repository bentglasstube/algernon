#pragma once

class Timer {

  public:

    Timer(unsigned int interval) : value_(0), interval_(interval) {}

    void update(unsigned int elapsed);

    bool fired() const { return overflow_; }
    unsigned int value() const { return value_; }

  private:

    unsigned int value_, interval_;
    bool overflow_;

};

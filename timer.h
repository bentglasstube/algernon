#pragma once

class Timer {

  public:

    Timer(unsigned int interval);
    void update(unsigned int elapsed);

    bool fired() const;
    unsigned int value() const;

  private:

    unsigned int value_, interval_;
    bool overflow_;

};

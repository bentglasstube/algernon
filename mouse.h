#pragma once

#include "entity.h"
#include "timer.h"

class Mouse : public MobileEntity {
  public:

    Mouse(Maze::Point p);

    void update(unsigned int elapsed, bool running);
    void draw(Graphics& graphics, int xo, int yo) const override;
    Rect hitbox() const override { return { x_ + (left_ ? 1 : 4 ), y_ + 4, 11, 9 }; }
    float satiety() const { return satiety_; }
    int lives() const { return lives_; }

    void feed() { satiety_ = std::min(8.0f, satiety_ + 1); }
    void hurt();
    bool invulnerable() { return iframes_ > 0; }

  private:

    static constexpr float kVelocity = 16 / 250.0f;
    static constexpr float kHungerRate = 0.005f;

    Timer animation_;
    float satiety_;
    int lives_, iframes_;

    int frame() const override { return moving() ? animation_.value() / 60 : 0; }

};

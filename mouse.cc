#include "mouse.h"

Mouse::Mouse(Maze::Point p) : MobileEntity(p), animation_(240), satiety_(8.0f), lives_(3), iframes_(0), power_(0) {}

void Mouse::update(unsigned int elapsed, bool running) {
  animation_.update(elapsed);
  if (iframes_ > 0) iframes_ -= elapsed;
  if (power_ > 0) power_ -= elapsed;
  const float delta = kVelocity * (running ? 2.5 : 1);
  move_toward_target(elapsed, delta);
  if (moving()) satiety_ = std::max(0.0f, satiety_ - kHungerRate * delta * (running ? 2 : 1));
}

void Mouse::draw(Graphics& graphics, int xo, int yo) const {
  if (iframes_ % 50 > 25 || iframes_ <= 0) MobileEntity::draw(graphics, xo, yo);
}

void Mouse::hurt() {
  lives_--;
  iframes_ = 1500;
}

void Mouse::powerup() {
  power_ = 10000;
}

int Mouse::frame() const {
  const int base = 0;
  const int powerup = 4 * ((power_ / 60) % 4);
  const int anim = animation_.value() / 60;

  return base + (power_ > 0 ? powerup : 0) + (moving() ? anim : 0);
}

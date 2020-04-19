#include "mouse.h"

Mouse::Mouse(Maze::Point p) : MobileEntity(p), animation_(240) {}

void Mouse::update(unsigned int elapsed) {
  animation_.update(elapsed);
  move_toward_target(elapsed, kVelocity);
}

int Mouse::frame() const {
  return moving() ? animation_.value() / 60: 0;
}

Rect Mouse::hitbox() const {
  return { x_ + (left_ ? 1 : 4 ), y_ + 4, 11, 9 };
}

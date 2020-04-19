#include "mouse.h"

Mouse::Mouse(Maze::Point p) : MobileEntity(p), animation_(240) {}

void Mouse::update(unsigned int elapsed) {
  animation_.update(elapsed);
  move_toward_target(elapsed, kVelocity);
}

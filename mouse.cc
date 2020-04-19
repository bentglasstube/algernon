#include "mouse.h"

Mouse::Mouse(Maze::Point p) : MobileEntity(p), animation_(240), satiety_(8.0f), lives_(3) {}

void Mouse::update(unsigned int elapsed, bool running) {
  animation_.update(elapsed);
  const float delta = kVelocity * (running ? 2.5 : 1);
  move_toward_target(elapsed, delta);
  if (moving()) satiety_ = std::max(0.0f, satiety_ - kHungerRate * delta * (running ? 2 : 1));
}

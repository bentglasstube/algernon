#include "mouse.h"

Mouse::Mouse(int x, int y) : MobileEntity(x, y), animation_(240) {}

void Mouse::update(unsigned int elapsed) {
  animation_.update(elapsed);

  const float delta = kVelocity * elapsed;

  if (x_ < tx_) {
    x_ = std::min(x_ + delta, tx_);
  } else if (x_ > tx_) {
    x_ = std::max(x_ - delta, tx_);
  } else if (y_ < ty_) {
    y_ = std::min(y_ + delta, ty_);
  } else if (y_ > ty_) {
    y_ = std::max(y_ - delta, ty_);
  }

}

int Mouse::frame() const {
  return moving() ? animation_.value() / 60: 0;
}

Rect Mouse::hitbox() const {
  return { x_ + (left_ ? 1 : 4 ), y_ + 4, 11, 9 };
}

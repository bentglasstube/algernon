#include "mouse.h"

Mouse::Mouse(int x, int y, bool left) :
  x_(x), y_(y), tx_(x), ty_(y), animation_(240), left_(left),
  sprites_("objects.png", 4, 16, 16)
{}

void Mouse::draw(Graphics& graphics, int xo, int yo) const {
  sprites_.draw_ex(graphics, animation_frame(), xo + x_, yo + y_, left_, 0, 0, 0);

#ifndef NDEBUG
  Rect h = hitbox();
  SDL_Rect r = {h.left(), h.top(), h.width(), h.height()};
  graphics.draw_rect(&r, 0x234599ff, false);
#endif
}

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

void Mouse::set_target(int x, int y) {
  tx_ = x * 16;
  ty_ = y * 16;

  if (tx_ < x_) left_ = true;
  if (tx_ > x_) left_ = false;
}

int Mouse::animation_frame() const {
  return moving() ? animation_.value() / 60: 0;
}

bool Mouse::moving() const {
  return x_ != tx_ || y_ != ty_;
}

int Mouse::mapx() const {
  return tx_ / 16;
}

int Mouse::mapy() const {
  return ty_ / 16;
}

Rect Mouse::hitbox() const {
  return { x_ + (left_ ? 1 : 4 ), y_ + 4, 11, 9 };
}

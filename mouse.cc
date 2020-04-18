#include "mouse.h"

Mouse::Mouse(int x, int y, bool left) :
  x_(x), y_(y), tx_(x), ty_(y_), timer_(0), left_(left),
  sprites_("objects.png", 4, 16, 16)
{}

void Mouse::draw(Graphics& graphics, int xo, int yo) const {
  // TODO support flipping
  sprites_.draw(graphics, animation_frame(), xo + x_, yo + y_);
}

void Mouse::update(unsigned int elapsed) {
  timer_ += elapsed;
  if (timer_ > 500) timer_ -= 500;

  // TODO move toward target
}

int Mouse::animation_frame() const {
  return timer_ / 125;
}

#include "object.h"

Object::Object(Object::Type type, int x, int y) :
  type_(type), x_(x * 16), y_(y * 16), timer_(0),
  sprites_("objects.png", 4, 16, 16)
{}

void Object::update(unsigned int elapsed) {
  timer_ += elapsed;
}

void Object::draw(Graphics& graphics, int xo, int yo) const {
  sprites_.draw(graphics, 8 + static_cast<int>(type_), x_ + xo, offset_y() + yo);

#ifndef NDEBUG
  const Rect h = hitbox();
  SDL_Rect r = {h.left() + xo, h.top() + yo, h.width(), h.height()};
  graphics.draw_rect(&r, 0xd8ff00ff, false);
#endif
}

Object::Type Object::type() const {
  return type_;
}

int Object::mapx() const {
  return x_ / 16;
}

int Object::mapy() const {
  return y_ / 16;
}

Rect Object::hitbox() const {
  return { x_ + 2, offset_y() + 2, 12, 12 };
}

float Object::offset_y() const {
  return y_ + 3 * std::sin(timer_ / 500.0f);
}

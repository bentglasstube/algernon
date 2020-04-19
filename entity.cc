#include "entity.h"

Entity::Entity(int x, int y) : x_(x * 16), y_(y * 16), sprites_("objects.png", 4, 16, 16) {}

void Entity::draw(Graphics& graphics, int xo, int yo) const {
  sprites_.draw(graphics, frame(), x() + xo, y() + yo);

#ifndef NDEBUG
  const Rect h = hitbox();
  SDL_Rect r = {h.left() + xo, h.top() + yo, h.width(), h.height()};
  graphics.draw_rect(&r, 0x00ffd8ff, false);
#endif
}

Rect Entity::hitbox() const {
  return { x() + 2, y() + 2, 12, 12 };
}

Maze::Point Entity::pos() const {
  return {x_ / 16, y_ / 16};
}

MobileEntity::MobileEntity(int x, int y) : Entity(x, y), tx_(x_), ty_(y_), left_(false) {}

void MobileEntity::draw(Graphics& graphics, int xo, int yo) const {
  sprites_.draw_ex(graphics, frame(), xo + x_, yo + y_, left_, 0, 0, 0);

#ifndef NDEBUG
  Rect h = hitbox();
  SDL_Rect r = {h.left() + xo, h.top() + yo, h.width(), h.height()};
  graphics.draw_rect(&r, 0x234599ff, false);
#endif
}

bool MobileEntity::moving() const {
  return x_ != tx_ || y_ != ty_;
}

void MobileEntity::set_target(int x, int y) {
  tx_ = x * 16;
  ty_ = y * 16;

  if (tx_ < x_) left_ = true;
  else if (tx_ > x_) left_ = false;
}

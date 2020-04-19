#include "entity.h"

Entity::Entity(int x, int y) : x_(x * 16), y_(y * 16), sprites_("objects.png", 4, 16, 16) {}

void Entity::draw(Graphics& graphics, int xo, int yo) const {
  sprites_.draw(graphics, frame(), x() + xo, y() + yo);

#ifndef NDEBUG
  SDL_Rect r = hitbox().offset_sdl_rect(xo, yo);
  graphics.draw_rect(&r, 0x00ffd8ff, false);
#endif
}

Rect Entity::hitbox() const {
  return { x() + 2, y() + 2, 12, 12 };
}

Maze::Point Entity::pos() const {
  return {(int)(x_ / 16), (int)(y_ / 16)};
}

MobileEntity::MobileEntity(int x, int y) : Entity(x, y), tx_(x_), ty_(y_), left_(false) {}

void MobileEntity::draw(Graphics& graphics, int xo, int yo) const {
  sprites_.draw_ex(graphics, frame(), xo + x_, yo + y_, left_, 0, 0, 0);

#ifndef NDEBUG
  SDL_Rect r = hitbox().offset_sdl_rect(xo, yo);
  graphics.draw_rect(&r, 0x234599ff, false);
#endif
}

bool MobileEntity::moving() const {
  return x_ != tx_ || y_ != ty_;
}

void MobileEntity::set_target(int x, int y) {
  tx_ = x * 16;
  ty_ = y * 16;
}

void MobileEntity::move_toward_target(unsigned int elapsed, float speed) {
  const float delta = speed * elapsed;

  if (x_ < tx_) {
    left_ = false;
    x_ = std::min(x_ + delta, tx_);
  } else if (x_ > tx_) {
    left_ = true;
    x_ = std::max(x_ - delta, tx_);
  } else if (y_ < ty_) {
    y_ = std::min(y_ + delta, ty_);
  } else if (y_ > ty_) {
    y_ = std::max(y_ - delta, ty_);
  }
}

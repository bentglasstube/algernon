#include "entity.h"

Entity::Entity(Maze::Point p) : x_(p.x * 16), y_(p.y * 16), sprites_("objects.png", 4, 16, 16) {}

void Entity::draw(Graphics& graphics, int xo, int yo) const {
  sprites_.draw(graphics, frame(), x() + xo, y() + yo);

#ifndef NDEBUG
  SDL_Rect r = hitbox().offset_sdl_rect(xo, yo);
  graphics.draw_rect(&r, 0x00ffd8ff, false);
#endif
}

MobileEntity::MobileEntity(Maze::Point p) : Entity(p), tx_(x_), ty_(y_), left_(false) {}

void MobileEntity::draw(Graphics& graphics, int xo, int yo) const {
  sprites_.draw_ex(graphics, frame(), xo + x_, yo + y_, left_, 0, 0, 0);

#ifndef NDEBUG
  SDL_Rect r = hitbox().offset_sdl_rect(xo, yo);
  graphics.draw_rect(&r, 0x234599ff, false);
#endif
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

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



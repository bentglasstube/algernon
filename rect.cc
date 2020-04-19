#include "rect.h"

bool Rect::intersect(const Rect& other) const {
  if (other.left() > right() || left() > other.right()) return false;
  if (other.top() > bottom() || top() > other.bottom()) return false;

  return true;
}

SDL_Rect Rect::offset_sdl_rect(int xo, int yo) const {
  return { (int)(left() + xo), (int)(top() + yo), (int)width(), (int)height() };
}

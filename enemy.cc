#include "enemy.h"

Enemy::Enemy(Enemy::Type type, int x, int y) :
  type_(type), x_(x * 16), y_(y * 16), timer_(400),
  sprites_("objects.png", 4, 16, 16)
{}

void Enemy::update(unsigned int elapsed, const Mouse& mouse) {
  timer_.update(elapsed);
  // TODO implement enemy AI
}

void Enemy::draw(Graphics& graphics, int xo, int yo) const {
  sprites_.draw_ex(graphics, animation_frame(), x_ + xo, y_ + yo, left_, 0, 0, 0);

#ifndef NDEBUG
  Rect h = hitbox();
  SDL_Rect r = {h.left() + xo, h.top() + yo, h.width(), h.height()};
  graphics.draw_rect(&r, 0x4488ccff, false);
#endif
}

bool Enemy::touching(const Mouse& mouse) const {
  return hitbox().intersect(mouse.hitbox());
}

Rect Enemy::hitbox() const {
  switch (type_) {
    case Type::Snake: return { x_ + (left_ ? 1 : 6), y_ + 1, 9, 14 };
    case Type::Spider: return { x_ + 5, y_ + 4, 6, 7 };
    default: return { x_, y_, 16, 16 };
  }
}

int Enemy::animation_frame() const {
  switch (type_) {
    case Type::Snake: return timer_.value() > 200 ? 4 : 5;
    case Type::Spider: return timer_.value() > 200 ? 6 : 7;
    default: return 99;
  }
}

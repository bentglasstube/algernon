#include "enemy.h"

#include "util.h"

Enemy::Enemy(Enemy::Type type, int x, int y) :
  type_(type),
  x_(x * 16), y_(y * 16), tx_(x_), ty_(y_),
  timer_(400), left_(false), charging_(false),
  idle_timer_(250),
  sprites_("objects.png", 4, 16, 16)
{
  rand_.seed(Util::random_seed());
}

void Enemy::update(unsigned int elapsed, const Mouse& mouse, const Maze& maze) {
  timer_.update(elapsed);

  const float delta = speed() * elapsed;

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

  if (x_ == tx_ && y_ == ty_) {
    if (idle_timer_ > 0) {
      idle_timer_ -= elapsed;
    } else if (type_ == Type::Spider) {
      idle_timer_ = 250;
      move_adjacent(maze);
    } else if (type_ == Type::Snake) {
      if (charging_) {
        idle_timer_ = 500;
        charging_ = false;
      } else if (maze.straight_path(pos(), mouse.pos())) {
        charge_position(mouse.pos());
      } else {
        move_adjacent(maze);
      }
    }
  }

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

float Enemy::speed() const {
  switch (type_) {
    case Type::Spider: return 16 / 250.0f;
    case Type::Snake: return 16 / 300.0f * (charging_ ? 2 : 1);
    default: return 16 / 500.0f;
  }
}

Maze::Point Enemy::pos() const {
  return { (int)(tx_ / 16), (int)(ty_ / 16) };
}

void Enemy::move_adjacent(const Maze& maze) {
  std::uniform_int_distribution<int> r(0, 3);
  const int dir = r(rand_);

  if (!maze.wall(pos(), dir)) {
    switch (dir) {
      case 0: ty_ = y_ - 16; break;
      case 1: tx_ = x_ + 16; break;
      case 2: ty_ = y_ + 16; break;
      case 3: tx_ = x_ - 16; break;
    }
  }
}

void Enemy::charge_position(Maze::Point p) {
  tx_ = p.x * 16;
  ty_ = p.y * 16;
  charging_ = true;
}

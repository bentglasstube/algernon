#include "enemy.h"

#include "util.h"

Enemy::Enemy(Enemy::Type type, Maze::Point p) :
  MobileEntity(p), type_(type),
  timer_(400), charging_(false), idle_timer_(250), exit_timer_(50000)
{
  rand_.seed(Util::random_seed());
}

void Enemy::update(unsigned int elapsed, const Mouse& mouse, const Maze& maze) {
  timer_.update(elapsed);
  move_toward_target(elapsed, speed());
  exit_timer_ -= elapsed;

  if (!moving()) {
    // If the exit timer has expired, try to leave the maze
    if (exit_timer_ < 0) {
      const auto p = pos();
      if (p.x == 0) {
        set_target({-1, p.y});
        return;
      } else if (p.y == 0) {
        set_target({p.x, -1});
        return;
      } else if (p.x == maze.width() - 1) {
        set_target({maze.width(), p.y});
        return;
      } else if (p.y == maze.height() - 1) {
        set_target({p.x, maze.height()});
        return;
      }
    }

    // Wait for idle timer if needed
    if (idle_timer_ > 0) {
      idle_timer_ -= elapsed;

    // Spiders should move randomly and then wait
    } else if (type_ == Type::Spider) {
      idle_timer_ = 250;
      move_adjacent(maze);

    // Snakes should charge the mouse if they see it
    // but they also need to rest if they just charged
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

int Enemy::frame() const {
  switch (type_) {
    case Type::Snake: return timer_.value() > 200 ? 16 : 17;
    case Type::Spider: return timer_.value() > 200 ? 18 : 19;
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

Rect Enemy::hitbox() const {
  switch (type_) {
    case Type::Snake: return { x_ + (left_ ? 1 : 6), y_ + 1, 9, 14 };
    case Type::Spider: return { x_ + 5, y_ + 4, 6, 7 };
    default: return MobileEntity::hitbox();
  }
}

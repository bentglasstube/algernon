#include "enemy.h"

#include "util.h"

Enemy::Enemy(Enemy::Type type, int x, int y) :
  MobileEntity(x, y), type_(type),
  timer_(400), charging_(false), idle_timer_(250)
{
  rand_.seed(Util::random_seed());
}

void Enemy::update(unsigned int elapsed, const Mouse& mouse, const Maze& maze) {
  timer_.update(elapsed);
  move_toward_target(elapsed, speed());

  if (!moving()) {
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

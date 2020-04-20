#pragma once

#include <random>

#include "entity.h"
#include "mouse.h"
#include "timer.h"

class Enemy : public MobileEntity {
  public:

    enum class Type { Snake, Spider };

    Enemy(Type type, Maze::Point p);
    virtual ~Enemy() {}

    void update(unsigned int elapsed, const Mouse& mouse, const Maze& maze);
    Rect hitbox() const override;

  private:

    Type type_;
    Timer timer_;
    bool charging_;
    int idle_timer_, exit_timer_;
    std::mt19937 rand_;

    int frame() const override;
    float speed() const;

    void move_adjacent(const Maze& maze);
    void charge_position(Maze::Point p);
};

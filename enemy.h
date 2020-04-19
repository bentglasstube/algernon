#pragma once

#include <random>

#include "graphics.h"
#include "spritemap.h"

#include "maze.h"
#include "mouse.h"
#include "rect.h"
#include "timer.h"

class Enemy {
  public:

    enum class Type { Snake, Spider };

    Enemy(Type type, int x, int y);

    void update(unsigned int elapsed, const Mouse& mouse, const Maze& maze);
    void draw(Graphics& graphics, int xo, int yo) const;

    bool touching(const Mouse& mouse) const;
    Rect hitbox() const;
    Maze::Point pos() const;

  private:

    Type type_;
    float x_, y_, tx_, ty_;
    Timer timer_;
    bool left_, charging_;
    int idle_timer_;
    SpriteMap sprites_;
    std::mt19937 rand_;

    int animation_frame() const;
    float speed() const;

    void move_adjacent(const Maze& maze);
    void charge_position(Maze::Point p);
};

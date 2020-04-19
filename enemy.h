#pragma once

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

  private:

    Type type_;
    float x_, y_, tx_, ty_;
    Timer timer_;
    bool left_;
    SpriteMap sprites_;

    int animation_frame() const;
};

#pragma once

#include "spritemap.h"

#include "timer.h"

class Mouse {
  public:

    Mouse(int x, int y, bool left);

    void draw(Graphics& graphics, int xo, int yo) const;
    void update(unsigned int elapsed);

    void set_target(int x, int y);

    bool moving() const;

    int mapx() const;
    int mapy() const;

  private:

    float x_, y_, tx_, ty_;
    Timer animation_;
    bool left_;

    SpriteMap sprites_;

    static constexpr float kVelocity = 16 / 250.0f;

    int animation_frame() const;

};

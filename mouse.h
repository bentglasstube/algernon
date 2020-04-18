#pragma once

#include "spritemap.h"

class Mouse {
  public:

    Mouse(int x, int y, bool left);

    void draw(Graphics& graphics, int xo, int yo) const;
    void update(unsigned int elapsed);

  private:

    int x_, y_, tx_, ty_;
    int timer_;
    bool left_;

    SpriteMap sprites_;

    int animation_frame() const;

};

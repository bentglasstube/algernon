#pragma once

#include "graphics.h"
#include "spritemap.h"

#include "maze.h"
#include "rect.h"

class Entity {
  public:

    Entity(int x, int y);

    virtual void update(unsigned int elapsed) = 0;
    virtual void draw(Graphics& graphics, int xo, int yo) const;

    virtual Rect hitbox() const;
    Maze::Point pos() const;

  protected:
    float x_, y_;
    SpriteMap sprites_;

    virtual float x() const { return x_; }
    virtual float y() const { return y_; }

    virtual int frame() const { return 0; }
};

class MobileEntity : public Entity {
  public:

    MobileEntity(int x, int y);

    virtual void draw(Graphics& graphics, int xo, int yo) const;
    bool moving() const;
    void set_target(int x, int y);

  protected:
    float tx_, ty_;
    bool left_;
};

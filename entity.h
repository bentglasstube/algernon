#pragma once

#include "graphics.h"
#include "spritemap.h"

#include "maze.h"
#include "rect.h"

class Entity {
  public:

    Entity(Maze::Point p);

    virtual void update(unsigned int) {}
    virtual void draw(Graphics& graphics, int xo, int yo) const;

    virtual Rect hitbox() const;
    Maze::Point pos() const;
    bool touching(const Entity& other) const { return hitbox().intersect(other.hitbox()); }

  protected:
    float x_, y_;
    SpriteMap sprites_;

    virtual float x() const { return x_; }
    virtual float y() const { return y_; }

    virtual int frame() const { return 0; }
};

class MobileEntity : public Entity {
  public:

    MobileEntity(Maze::Point p);

    virtual void draw(Graphics& graphics, int xo, int yo) const;
    bool moving() const;
    void set_target(Maze::Point p);

    void move_toward_target(unsigned int elapsed, float speed);

  protected:
    float tx_, ty_;
    bool left_;
};

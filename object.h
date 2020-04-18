#pragma once

#include "graphics.h"
#include "spritemap.h"

class Object {
  public:

    enum class Type { Cheese, Droplet, Leaf, Mushroom, Seed };

    Object(Type type, int x, int y);

    void update(unsigned int elapsed);
    void draw(Graphics& graphics) const;

    Type type() const;
    int mapx() const;
    int mapy() const;

  private:

    Type type_;
    float x_, y_;
    int timer_;
    SpriteMap sprites_;
};

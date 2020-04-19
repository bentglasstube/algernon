#pragma once

#include "entity.h"

class PowerUp : public Entity {
  public:

    enum class Type { Cheese, Droplet, Leaf, Mushroom };

    PowerUp(Type type, Maze::Point p) : Entity(p), type_(type), timer_(0) {};

    void update(unsigned int elapsed) { timer_ += elapsed; }

    Type type() const { return type_; }

  private:

    Type type_;
    int timer_;

    float y() const override { return y_ + 3 * std::sin(timer_ / 500.0f); }
    int frame() const override { return 20 + static_cast<int>(type_); }
};

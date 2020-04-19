#pragma once

#include "entity.h"

class PowerUp : public Entity {
  public:

    enum class Type { Cheese, Droplet, Leaf, Mushroom };

    PowerUp(Type type, int x, int y);

    void update(unsigned int elapsed) override;

    Type type() const;

  private:

    Type type_;
    int timer_;

    float y() const override;
    int frame() const override;
};

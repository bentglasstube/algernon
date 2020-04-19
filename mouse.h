#pragma once

#include "entity.h"
#include "timer.h"

class Mouse : public MobileEntity {
  public:

    Mouse(Maze::Point p);

    void update(unsigned int elapsed) override;
    Rect hitbox() const override;

  private:

    static constexpr float kVelocity = 16 / 250.0f;

    Timer animation_;

    int frame() const override;

};

#pragma once

#include "entity.h"

class Flower : public Entity {
  public:

    Flower(Maze::Point p);

    void update(unsigned int elapsed) override;

    float water() const { return water_; }
    float nutrients() const { return nutrients_; }
    float growth() const { return growth_; }

    void give_water() { water_ = std::min(water_ + 1, 8.0f); }
    void give_composte() { nutrients_ = std::min(nutrients_ + 1, 8.0f); }

  private:

    static constexpr float kDecayRate = 1 / 5000.0f;      // 1 point every 5 seconds
    static constexpr float kGrowthRate = 4 / 300000.0f;   // 4 points over 5 minutes

    float water_, nutrients_, growth_;

    int frame() const override { return (int)(12 + growth_); }

};

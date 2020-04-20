#pragma once

#include "entity.h"

class Flower : public Entity {
  public:

    Flower(Maze::Point p) : Entity(p), water_(5), nutrients_(5), growth_(0) {}

    void update(unsigned int elapsed);

    float water() const { return water_; }
    float nutrients() const { return nutrients_; }
    float growth() const { return growth_; }

    float growth_rate() const { return kGrowthRate * std::log(std::min(water_, nutrients_) + 1); }

    void give_water(float value) { water_ = std::min(water_ + value, 8.0f); }
    void give_composte(float value) { nutrients_ = std::min(nutrients_ + value, 8.0f); }

  private:

    static constexpr float kDecayRate = 1 / 20000.0f;     // 1 point every 20 seconds
    static constexpr float kGrowthRate = 4 / 300000.0f;   // 4 points over 5 minutes

    float water_, nutrients_, growth_;

    int frame() const override { return 24 + std::min((int)growth_, 3); }

};

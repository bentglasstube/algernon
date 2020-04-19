#include "flower.h"

Flower::Flower(Maze::Point p) : Entity(p), water_(5), nutrients_(5), growth_(0) {}

void Flower::update(unsigned int elapsed) {
  water_ = std::max(0.0f, water_ - kDecayRate * elapsed);
  nutrients_ = std::max(0.0f, nutrients_ - kDecayRate * elapsed);
  growth_ += std::min(8.0f, kGrowthRate * elapsed);
}

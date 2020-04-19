#include "flower.h"

Flower::Flower(Maze::Point p) : Entity(p), water_(5), nutrients_(5), growth_(0) {}

void Flower::update(unsigned int elapsed) {
  water_ -= kDecayRate * elapsed;
  nutrients_ -= kDecayRate * elapsed;
  growth_ += kGrowthRate * elapsed;
}

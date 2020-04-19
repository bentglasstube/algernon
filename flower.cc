#include "flower.h"

Flower::Flower(int x, int y) : Entity(x, y), water_(5), nutrients_(5), growth_(0) {}

void Flower::update(unsigned int elapsed) {
  water_ -= kDecayRate * elapsed;
  nutrients_ -= kDecayRate * elapsed;
  growth_ += kGrowthRate * elapsed;
}

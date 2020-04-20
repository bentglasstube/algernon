#include "flower.h"

void Flower::update(unsigned int elapsed) {
  water_ = std::max(0.0f, water_ - kDecayRate * elapsed);
  nutrients_ = std::max(0.0f, nutrients_ - kDecayRate * elapsed);
  growth_ += std::min(4.0f,  growth_rate() * elapsed);
}

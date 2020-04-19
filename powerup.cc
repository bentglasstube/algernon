#include "powerup.h"

PowerUp::PowerUp(PowerUp::Type type, Maze::Point p) : Entity(p), type_(type), timer_(0) {}

void PowerUp::update(unsigned int elapsed) {
  timer_ += elapsed;
}

PowerUp::Type PowerUp::type() const {
  return type_;
}

float PowerUp::y() const {
  return y_ + 3 * std::sin(timer_ / 500.0f);
}

int PowerUp::frame() const {
  return 8 + static_cast<int>(type_);
}

#include "powerup.h"

void PowerUp::draw(Graphics& graphics, int xo, int yo) const {
  if (timer_ < kDespawnTime - kBlinkTime || (timer_ / kBlinkRate) % 2 == 1) {
    Entity::draw(graphics, xo, yo);
  }
}

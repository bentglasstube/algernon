#include "maze_screen.h"

#include <algorithm>

#include "util.h"

MazeScreen::MazeScreen() :
  text_("text.png"),
  maze_(16, 14),
  mouse_(0, 0),
  spawner_(5000),
  item_(nullptr)
{
  maze_.generate();
  rand_.seed(Util::random_seed());
}

bool MazeScreen::update(const Input& input, Audio&, unsigned int elapsed) {
  if (!mouse_.moving()) {
    if (input.key_held(Input::Button::Up)) {
      try_to_move(0);
    } else if (input.key_held(Input::Button::Right)) {
      try_to_move(1);
    } else if (input.key_held(Input::Button::Down)) {
      try_to_move(2);
    } else if (input.key_held(Input::Button::Left)) {
      try_to_move(3);
    }
  }

  if (item_) item_->update(elapsed);

  powerups_.erase(std::remove_if( powerups_.begin(), powerups_.end(),
      [this](const PowerUp& p){
        if (p.hitbox().intersect(mouse_.hitbox())) {
          return powerup(p.type());
        } else {
          return false;
        }
      }), powerups_.end());

  mouse_.update(elapsed);

  spawner_.update(elapsed);

  if (spawner_.fired()) {
    std::uniform_int_distribution<int> rx(0, maze_.width() - 1);
    std::uniform_int_distribution<int> ry(0, maze_.height() - 1);

    if (powerups_.size() < 3) {
      // Pick a random number 0 - 9 which will be devided by 3
      // This gives 30% chance each for Cheese, Water, and Leaf, and 10% chance for Mushroom
      std::uniform_int_distribution<int> rd(0, 9);
      powerups_.emplace_back(static_cast<PowerUp::Type>(rd(rand_) / 3), rx(rand_), ry(rand_));
    }

    if (enemies_.size() < 2) {
      std::uniform_int_distribution<int> rs(0, 3);
      std::uniform_int_distribution<int> re(0, 1);

      const int side = rs(rand_);

      switch (side) {
        case 0:
          enemies_.emplace_back(static_cast<Enemy::Type>(re(rand_)), rx(rand_), 0);
          break;

        case 1:
          enemies_.emplace_back(static_cast<Enemy::Type>(re(rand_)), maze_.width() - 1, ry(rand_));
          break;

        case 2:
          enemies_.emplace_back(static_cast<Enemy::Type>(re(rand_)), rx(rand_), maze_.height() - 1);
          break;

        case 3:
          enemies_.emplace_back(static_cast<Enemy::Type>(re(rand_)), 0, ry(rand_));
          break;

      }
    }
  }

  for (auto& p : powerups_) {
    p.update(elapsed);
  }

  for (auto& e : enemies_) {
    e.update(elapsed, mouse_, maze_);
  }

  return true;
}

void MazeScreen::draw(Graphics& graphics) const {
  maze_.draw(graphics, 0, 16);
  for (const auto& p : powerups_) {
    p.draw(graphics, 0, 16);
  }
  for (const auto& e : enemies_) {
    e.draw(graphics, 0, 16);
  }
  mouse_.draw(graphics, 0, 16);

  // TODO draw HUD elements
  if (item_) {
    item_->draw(graphics, 120, 0);
  }
}

Screen* MazeScreen::next_screen() const {
  return nullptr;
}

void MazeScreen::try_to_move(int direction) {
  auto mp = mouse_.pos();

  if (maze_.wall(mp, direction)) return;

  switch (direction) {
    case 0: mouse_.set_target(mp.x, mp.y - 1); break;
    case 1: mouse_.set_target(mp.x + 1, mp.y); break;
    case 2: mouse_.set_target(mp.x, mp.y + 1); break;
    case 3: mouse_.set_target(mp.x - 1, mp.y); break;
  }
}

bool MazeScreen::powerup(PowerUp::Type type) {
  switch (type) {
    case PowerUp::Type::Cheese:
      // TODO increase mouse satiety
      return true;

    case PowerUp::Type::Droplet:
    case PowerUp::Type::Leaf:
      if (item_) {
        return false;
      } else {
        item_.reset(new PowerUp(type, 0, 0));
        return true;
      }

    case PowerUp::Type::Mushroom:
      // TODO grant invulnerability to mouse
      return true;

    default:
      return false;
  }
}

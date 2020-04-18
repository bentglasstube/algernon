#include "maze_screen.h"

#include <algorithm>

#include "util.h"

MazeScreen::MazeScreen() :
  text_("text.png"),
  maze_(16, 15),
  mouse_(0, 0, false),
  spawner_(5000)
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

  // TODO sometimes this doesn't pick things up, improve code
  std::remove_if( objects_.begin(), objects_.end(),
      [this](Object const& o){
        if (o.mapx() == mouse_.mapx() && o.mapy() == mouse_.mapy()) {
          // TODO grant powerup
          return true;
        } else {
          return false;
        }
      });

  mouse_.update(elapsed);

  spawner_.update(elapsed);

  if (spawner_.fired()) {
    std::uniform_int_distribution<int> rx(0, maze_.width() - 1);
    std::uniform_int_distribution<int> ry(0, maze_.height() - 1);

    // Pick a random number 0 - 9 which will be devided by 3
    // This gives 30% chance each for Cheese, Water, and Leaf, and 10% chance for Mushroom
    std::uniform_int_distribution<int> rd(0, 9);

    objects_.emplace_back(static_cast<Object::Type>(rd(rand_) / 3), rx(rand_), ry(rand_));
  }

  for (auto& o : objects_) {
    o.update(elapsed);
  }

  return true;
}

void MazeScreen::draw(Graphics& graphics) const {
  maze_.draw(graphics, 0, 0);
  for (const auto& o : objects_) {
    o.draw(graphics);
  }
  mouse_.draw(graphics, 0, 0);
}

Screen* MazeScreen::next_screen() const {
  return nullptr;
}

void MazeScreen::try_to_move(int direction) {
  const int mx = mouse_.mapx();
  const int my = mouse_.mapy();

  if (maze_.wall(mx, my, direction)) return;

  switch (direction) {
    case 0: mouse_.set_target(mx, my - 1); break;
    case 1: mouse_.set_target(mx + 1, my); break;
    case 2: mouse_.set_target(mx, my + 1); break;
    case 3: mouse_.set_target(mx - 1, my); break;
  }
}

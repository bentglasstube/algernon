#include "maze_screen.h"

MazeScreen::MazeScreen() : text_("text.png"), maze_(16, 15), mouse_(0, 0, false) {
  maze_.generate();
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

  mouse_.update(elapsed);

  return true;
}

void MazeScreen::draw(Graphics& graphics) const {
  maze_.draw(graphics, 0, 0);
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

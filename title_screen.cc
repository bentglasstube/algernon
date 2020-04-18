#include "title_screen.h"

TitleScreen::TitleScreen() : text_("text.png"), maze_(16, 15), generator_(50), flash_(1000) {}

bool TitleScreen::update(const Input& input, Audio&, unsigned int elapsed) {
  flash_.update(elapsed);
  generator_.update(elapsed);

  if (generator_.fired()) {
    maze_.step();
  }

  if (input.any_pressed()) {
    return false;
  }

  return true;
}

void TitleScreen::draw(Graphics& graphics) const {
  maze_.draw(graphics, 0, 48);
  if (flash_.value() < 500) text_.draw(graphics, "Press any key", 128, 216, Text::Alignment::Center);
}

Screen* TitleScreen::next_screen() const {
  return nullptr;
}

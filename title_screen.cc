#include "title_screen.h"

#include "maze_screen.h"

TitleScreen::TitleScreen() :
  text_("text.png"), title_("title.png", 0, 0, 256, 128),
  flower_({14, 3}),
  maze_(16, 15), generator_(25), flash_(1000)
{
  maze_.add_title_steps();
}

bool TitleScreen::update(const Input& input, Audio&, unsigned int elapsed) {
  flash_.update(elapsed);
  generator_.update(elapsed);
  flower_.update(elapsed * 100);

  if (generator_.fired()) maze_.step();

  return !input.any_pressed();
}

void TitleScreen::draw(Graphics& graphics) const {
  maze_.draw(graphics, 0, 0);
  flower_.draw(graphics, 0, 0);

  if (maze_.letters_done()) title_.draw(graphics, 0, 48);
  if (flash_.value() < 500) text_.draw(graphics, "Press any key", 128, 216, Text::Alignment::Center);
}

Screen* TitleScreen::next_screen() const {
  return new MazeScreen();
}

#include "title_screen.h"

#include "maze_screen.h"

TitleScreen::TitleScreen() :
  text_("text.png"), title_("title.png", 0, 0, 256, 128),
  flower_({14, 3}),
  maze_(16, 15), generator_(25), flash_(1000),
  scroll_timer_(0), scroll_(0)
{
  maze_.add_title_steps();

  roll_.add_entry(new TitleRoll::Title("How to Play"));
  roll_.add_entry(new TitleRoll::NamedItem("Algernon", 0, 3));
  roll_.add_entry(new TitleRoll::Description("Navigate the maze in the lab\nto collect items and help his\nflower grow."));
  roll_.add_entry(new TitleRoll::Description("Algernon can run by holding\nJ or X, but this will make\nhim get tired more quickly."));
  roll_.add_entry(new TitleRoll::NamedItem("Flower", 27));
  roll_.add_entry(new TitleRoll::Description("Starts out small but grows\nover time. Make sure it gets\nenough water and nutrients."));
  roll_.add_entry(new TitleRoll::NamedItem("Dead Leaf", 22));
  roll_.add_entry(new TitleRoll::Description("Provides much needed nutrients\nfor the soil."));
  roll_.add_entry(new TitleRoll::NamedItem("Water Droplet", 21));
  roll_.add_entry(new TitleRoll::Description("Plants need water to grow."));
  roll_.add_entry(new TitleRoll::NamedItem("Cheese", 20));
  roll_.add_entry(new TitleRoll::Description("Gives Algernon more energy.\nDon't starve!"));
  roll_.add_entry(new TitleRoll::NamedItem("Mushroom", 23));
  roll_.add_entry(new TitleRoll::Description("Bestows unusual powers when\neaten. While affected, even a\nmouse is unstoppable."));
  roll_.add_entry(new TitleRoll::NamedItem("Spider", 18, 19));
  roll_.add_entry(new TitleRoll::Description("Mostly harmless creatures that\nwander the maze. They will bite\nanything that gets close."));
  roll_.add_entry(new TitleRoll::NamedItem("Snake", 16, 17));
  roll_.add_entry(new TitleRoll::Description("These predators will charge at\ntheir prey when they can see it.\nBe careful around them."));
}

bool TitleScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  audio.play_music("cyberhog.mp3", true);

  flash_.update(elapsed);
  generator_.update(elapsed);
  flower_.update(elapsed * 100);
  roll_.update(elapsed);

  if (generator_.fired()) maze_.step();

  scroll_timer_ += elapsed;
  if (scroll_timer_ > kScrollDelay) {
    scroll_ += kScrollRate * elapsed;
    // TODO end of scroll
  }

  return !input.any_pressed();
}

void TitleScreen::draw(Graphics& graphics) const {
  maze_.draw(graphics, 0, -scroll_);
  flower_.draw(graphics, 0, -scroll_);

  if (maze_.letters_done()) title_.draw(graphics, 0, 48 - scroll_);
  if (flash_.value() < 500 && scroll_ == 0) text_.draw(graphics, "Press any key", 128, 216, Text::Alignment::Center);

  if (scroll_ > 0) {
    roll_.draw(graphics, 256 - scroll_);
  }
}

Screen* TitleScreen::next_screen() const {
  return new MazeScreen();
}

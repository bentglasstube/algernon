#pragma once

#include "screen.h"
#include "text.h"

#include "maze.h"
#include "timer.h"

class TitleScreen : public Screen {
  public:

    TitleScreen();

    bool update(const Input& input, Audio& audio, unsigned int elapsed) override;
    void draw(Graphics& graphics) const override;

    Screen* next_screen() const override;

  private:

    Text text_;
    Maze maze_;
    Timer generator_, flash_;
};

#pragma once

#include "screen.h"
#include "sprite.h"
#include "text.h"

#include "flower.h"
#include "maze.h"
#include "timer.h"
#include "title_roll.h"

class TitleScreen : public Screen {
  public:

    TitleScreen();

    bool update(const Input& input, Audio& audio, unsigned int elapsed) override;
    void draw(Graphics& graphics) const override;

    Screen* next_screen() const override;

  private:

    static constexpr int kScrollDelay = 10000;
    static constexpr float kScrollRate = 16 / 1000.0f;

    Text text_;
    Sprite title_;
    Flower flower_;

    Maze maze_;
    Timer generator_, flash_;

    int scroll_timer_;
    float scroll_;
    TitleRoll roll_;
};

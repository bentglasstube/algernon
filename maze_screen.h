#pragma once

#include <memory>
#include <random>
#include <vector>

#include "screen.h"

#include "enemy.h"
#include "maze.h"
#include "mouse.h"
#include "powerup.h"
#include "text.h"
#include "timer.h"

class MazeScreen : public Screen {
  public:

    MazeScreen();

    bool update(const Input& input, Audio& audio, unsigned int elapsed) override;
    void draw(Graphics& graphics) const override;

    Screen* next_screen() const override;

  private:

    Text text_;
    Maze maze_;
    Mouse mouse_;
    Timer spawner_;

    std::unique_ptr<PowerUp> item_;

    std::vector<PowerUp> powerups_;
    std::vector<Enemy> enemies_;
    std::mt19937 rand_;

    void try_to_move(int direction);
    bool powerup(PowerUp::Type type);
};

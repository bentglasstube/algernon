#pragma once

#include <memory>
#include <random>
#include <vector>

#include "screen.h"
#include "spritemap.h"
#include "text.h"

#include "enemy.h"
#include "flower.h"
#include "maze.h"
#include "mouse.h"
#include "powerup.h"
#include "timer.h"

class MazeScreen : public Screen {
  public:

    MazeScreen();

    bool update(const Input& input, Audio& audio, unsigned int elapsed) override;
    void draw(Graphics& graphics) const override;

    Screen* next_screen() const override;

  private:

    static constexpr int kHUDHeight = 16;

    enum class State { Playing, Paused, Outro };
    enum class Result { None, Killed, Starved, Stunted, Wilted, Grew };

    Text text_;
    SpriteMap ui_;

    Maze maze_;
    Mouse mouse_;
    Timer spawner_;
    Flower flower_;
    State state_;
    Result result_;
    std::unique_ptr<PowerUp> item_;
    int fadeout_;

    std::vector<PowerUp> powerups_;
    std::vector<Enemy> enemies_;
    std::mt19937 rand_;

    void try_to_move(int direction);
    bool powerup(PowerUp::Type type, Audio& audio);

    void draw_ui(Graphics& graphics) const;
    void histogram(Graphics& graphics, int base, float value, int x, int y, bool reverse) const;
    void set_result(Result r);
};

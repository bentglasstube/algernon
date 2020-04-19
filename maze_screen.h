#pragma once

#include <memory>
#include <random>
#include <unordered_map>
#include <vector>

#include "screen.h"
#include "spritemap.h"
#include "text.h"

#include "appearing_text.h"
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

    enum class State { Playing, Paused, Outro };
    enum class Result { None, Killed, Starved, Stunted, Wilted, Grew };

    static constexpr int kHUDHeight = 16;
    static constexpr int kFadeTime = 3000;
    static const std::unordered_map<Result, std::pair<int, std::string>, Util::CastHash<Result>> kEnding;

    Text text_, black_text_;
    SpriteMap ui_;
    SpriteMap endings_;

    Maze maze_;
    Mouse mouse_;
    Timer spawner_;
    Flower flower_;
    State state_;
    Result result_;
    int fadeout_;

    std::unique_ptr<PowerUp> item_;
    std::unique_ptr<AppearingText> story_;
    std::vector<PowerUp> powerups_;
    std::vector<Enemy> enemies_;
    std::mt19937 rand_;

    void try_to_move(int direction);
    bool powerup(PowerUp::Type type, Audio& audio);

    void draw_ui(Graphics& graphics) const;
    void histogram(Graphics& graphics, int base, float value, int x, int y, bool reverse) const;
    void set_result(Result r);
    int ending() const;
};

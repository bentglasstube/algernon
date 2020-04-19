#include "maze_screen.h"

#include <algorithm>

#include "util.h"

MazeScreen::MazeScreen() :
  text_("text.png"), ui_("ui.png", 4, 8, 8),
  maze_(16, 14), mouse_({0, 0}), spawner_(5000),
  flower_({maze_.width() / 2, maze_.height() / 2}),
  item_()
{
  maze_.generate();
  rand_.seed(Util::random_seed());
}

bool MazeScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
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

  if (input.key_pressed(Input::Button::Select) && item_) {
    item_.reset();
    audio.play_sample("drop.wav");
  }

  for (auto& p : powerups_) { p.update(elapsed); }
  for (auto& e : enemies_) { e.update(elapsed, mouse_, maze_); }
  mouse_.update(elapsed, input.key_held(Input::Button::B));
  flower_.update(elapsed);
  spawner_.update(elapsed);

  powerups_.erase(std::remove_if( powerups_.begin(), powerups_.end(),
      [this, &audio](const PowerUp& p){
        if (p.touching(mouse_)) {
          return powerup(p.type(), audio);
        } else {
          return false;
        }
      }), powerups_.end());

  if (spawner_.fired()) {
    std::uniform_int_distribution<int> rx(0, maze_.width() - 1);
    std::uniform_int_distribution<int> ry(0, maze_.height() - 1);

    if (powerups_.size() < 5) {
      // Pick a random number 0 - 9 which will be devided by 3
      // This gives 30% chance each for Cheese, Water, and Leaf, and 10% chance for Mushroom
      std::uniform_int_distribution<int> rd(0, 9);
      const Maze::Point spawn = { rx(rand_), ry(rand_) };
      powerups_.emplace_back(static_cast<PowerUp::Type>(rd(rand_) / 3), spawn);
    }

    if (enemies_.size() < 3) {
      std::uniform_int_distribution<int> rs(0, 3);
      std::uniform_int_distribution<int> re(0, 1);

      const int side = rs(rand_);

      Maze::Point spawn = { 0, 0 };

      switch (side) {
        case 0:
          spawn.x = rx(rand_);
          break;

        case 1:
          spawn.x = maze_.width() - 1;
          spawn.y = ry(rand_);
          break;

        case 2:
          spawn.x = rx(rand_);
          spawn.y = maze_.height() - 1;
          break;

        case 3:
          spawn.y = ry(rand_);
          break;
      }

      enemies_.emplace_back(static_cast<Enemy::Type>(re(rand_)), spawn);
    }
  }

  if (mouse_.touching(flower_) && item_) {
    switch (item_->type()) {
      case PowerUp::Type::Droplet:
        flower_.give_water();
        item_.reset();
        audio.play_sample("boost.wav");
        break;

      case PowerUp::Type::Leaf:
        flower_.give_composte();
        item_.reset();
        audio.play_sample("boost.wav");
        break;

      default:
        break;
    }
  }

  if (!mouse_.invulnerable()) {
    enemies_.erase(std::remove_if( enemies_.begin(), enemies_.end(),
          [this, &audio](const Enemy& e){
            if (e.touching(mouse_)) {
              if (mouse_.powered_up()) {
                audio.play_sample("bite.wav");
                return true;
              } else {
                mouse_.hurt();
                audio.play_sample("hurt.wav");
                return false;
              }
            }
            return false;
          }), enemies_.end());
  }

  return true;
}

void MazeScreen::draw(Graphics& graphics) const {
  maze_.draw(graphics, 0, 16);
  for (const auto& p : powerups_) {
    p.draw(graphics, 0, 16);
  }
  for (const auto& e : enemies_) {
    e.draw(graphics, 0, 16);
  }
  flower_.draw(graphics, 0, 16);
  mouse_.draw(graphics, 0, 16);

  draw_ui(graphics);
}

void MazeScreen::draw_ui(Graphics& graphics) const {
  if (item_) {
    ui_.draw(graphics, 1, 112, 0);
    ui_.draw(graphics, 2, 112, 8);
    ui_.draw_ex(graphics, 1, 136, 0, true, 0, 0, 0);
    ui_.draw_ex(graphics, 2, 136, 8, true, 0, 0, 0);

    item_->draw(graphics, 120, 0);
  }

  histogram(graphics, 0, mouse_.lives(), 0, 0, false);
  histogram(graphics, 12, mouse_.satiety(), 0, 8, false);
  histogram(graphics, 4, flower_.water(), 248, 0, true);
  histogram(graphics, 8, flower_.nutrients(), 248, 8, true);

#ifndef NDEBUG
  text_.draw(graphics, "W: " + std::to_string((int)std::round(flower_.water() * 100)), 0, 192);
  text_.draw(graphics, "N: " + std::to_string((int)std::round(flower_.nutrients() * 100)), 0, 208);
  text_.draw(graphics, "G: " + std::to_string((int)std::round(flower_.growth() * 100)), 0, 224);
#endif
}

void MazeScreen::histogram(Graphics& graphics, int base, float value, int x, int y, bool reverse) const {
  int v = std::round(value * -4);
  for (int i = 0; i < 8; ++i) {
    v += 4;
    const int n = base + std::max(0, std::min(3, v));
    ui_.draw(graphics, n, x + i * (reverse ? -8 : 8), y);
  }
}

Screen* MazeScreen::next_screen() const {
  return nullptr;
}

void MazeScreen::try_to_move(int direction) {
  auto mp = mouse_.pos();

  if (maze_.wall(mp, direction)) return;

  switch (direction) {
    case 0: mouse_.set_target({mp.x, mp.y - 1}); break;
    case 1: mouse_.set_target({mp.x + 1, mp.y}); break;
    case 2: mouse_.set_target({mp.x, mp.y + 1}); break;
    case 3: mouse_.set_target({mp.x - 1, mp.y}); break;
  }
}

bool MazeScreen::powerup(PowerUp::Type type, Audio& audio) {
  switch (type) {
    case PowerUp::Type::Cheese:
      mouse_.feed();
      audio.play_sample("pickup.wav");
      return true;

    case PowerUp::Type::Droplet:
    case PowerUp::Type::Leaf:
      if (item_) {
        return false;
      } else {
        item_.reset(new PowerUp(type, {0, 0}));
        audio.play_sample("pickup.wav");
        return true;
      }

    case PowerUp::Type::Mushroom:
      mouse_.powerup();
      audio.play_sample("shroom.wav");
      return true;

    default:
      return false;
  }
}

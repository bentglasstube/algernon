#include "maze_screen.h"

#include <algorithm>

#include "util.h"

#include "title_screen.h"

MazeScreen::MazeScreen() :
  text_("text.png"), black_text_("black_text.png"),
  ui_("ui.png", 4, 8, 8), endings_("ending.png", 1, 160, 112),
  maze_(16, 14), spawner_(5000),
  mouse_(maze_.center_pos()), flower_(maze_.center_pos()),
  state_(State::Playing), result_(Result::None), fadeout_(0)
{
  maze_.generate();
  maze_.open_middle();
  rand_.seed(Util::random_seed());
}

bool MazeScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  switch (state_) {

    case State::Paused:

      if (input.key_pressed(Input::Button::Start)) {
        audio.play_sample("pause.wav");
        state_ = State::Playing;
      }
      break;

    case State::Outro:

      if (fadeout_ > kFadeTime) {
        story_->update(audio, elapsed);
      } else {
        fadeout_ += elapsed;
      }

      if (story_->done() && input.any_pressed()) return false;

      break;

    case State::Playing:

      if (input.key_pressed(Input::Button::Start)) {
        audio.play_sample("pause.wav");
        state_ = State::Paused;
      }

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
            if (p.touching(mouse_)) return powerup(p.type(), audio);
            return p.despawn();
          }), powerups_.end());

      if (spawner_.fired()) {
        std::uniform_int_distribution<size_t> drop(0, powerups_.size() + enemies_.size() + 1);
        if (drop(rand_) <= enemies_.size()) {
          spawn_powerup();
        } else {
          spawn_enemy();
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

      enemies_.erase(std::remove_if( enemies_.begin(), enemies_.end(),
            [this, &audio](const Enemy& e){
              if (!maze_.valid(e.pos())) return true;
              if (e.touching(mouse_)) {
                if (mouse_.powered_up()) {
                  audio.play_sample("bite.wav");
                  return true;
                } else if (!mouse_.invulnerable()) {
                  mouse_.hurt();
                  audio.play_sample("hurt.wav");
                  return false;
                }
              }
              return false;
            }), enemies_.end());

      // Win and loss conditions
      if (mouse_.lives() <= 0)      set_result(audio, Result::Killed);
      if (mouse_.satiety() <= 0)    set_result(audio, Result::Starved);
      if (flower_.nutrients() <= 0) set_result(audio, Result::Stunted);
      if (flower_.water() <= 0)     set_result(audio, Result::Wilted);
      if (flower_.growth() >= 4)    set_result(audio, Result::Grew);

      break;

  }

  return true;
}

void MazeScreen::draw(Graphics& graphics) const {
  maze_.draw(graphics, 0, kHUDHeight);
  for (const auto& p : powerups_) { p.draw(graphics, 0, kHUDHeight); }
  for (const auto& e : enemies_) { e.draw(graphics, 0, kHUDHeight); }
  flower_.draw(graphics, 0, kHUDHeight);
  mouse_.draw(graphics, 0, kHUDHeight);

  draw_ui(graphics);

  if (state_ == State::Paused) {

    SDL_Rect r = { 0, kHUDHeight, graphics.width(), graphics.height() - kHUDHeight };
    graphics.draw_rect(&r, 0x00000099, true);
    text_.draw(graphics, "Paused", 128, 112, Text::Alignment::Center);

  } else if (state_ == State::Outro) {

    const int fade = std::min(255, 256 * fadeout_ / kFadeTime) + (result_ == Result::Grew ? 0xffffff00 : 0);
    SDL_Rect r = {0, 0, graphics.width(), graphics.height()};
    graphics.draw_rect(&r, fade, true);

    if (fadeout_ > kFadeTime) {
      story_->draw(graphics, (result_ == Result::Grew ? black_text_ : text_), 0, 176);
      endings_.draw(graphics, kEnding.at(result_).first, 48, 48);
    }
  }
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
    ui_.draw(graphics, base + Util::clamp(v, 0, 3), x + i * (reverse ? -8 : 8), y);
  }
}

Screen* MazeScreen::next_screen() const {
  return new TitleScreen();
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

void MazeScreen::set_result(Audio& audio, Result r) {
  result_ = r;
  state_ = State::Outro;
  story_.reset(new AppearingText(kEnding.at(result_).second));

  if (r == Result::Grew) {
    audio.play_music("springtime.ogg", true);
  } else {
    audio.play_music("schvitz.ogg", false);
  }
}

const std::unordered_map<MazeScreen::Result, std::pair<int, std::string>, Util::CastHash<MazeScreen::Result>> MazeScreen::kEnding = {
  { Result::None,    { -1, "How did you get here?" }},
  { Result::Killed,  { 0, "Alas, poor Algernon was killed\nby the monsters that lurked in\nthe laboratory." }},
  { Result::Starved, { 0, "Poor Algernon was so focused on\nkeeping his flower alive that he\nforgot to eat." }},
  { Result::Wilted,  { 2, "Try as he might, Algernon could\nnot keep his flower alive." }},
  { Result::Stunted, { 2, "Try as he might, Algernon could\nnot keep his flower alive." }},
  { Result::Grew,    { 1, "Algernon took care of his flower\nmeticulously.  In the end, it\ngrew strong and beautiful." }},
};

void MazeScreen::spawn_powerup() {
  std::vector<PowerUp::Type> weights;

  // default weights
  weights.push_back(PowerUp::Type::Cheese);
  weights.push_back(PowerUp::Type::Droplet);
  weights.push_back(PowerUp::Type::Leaf);
  weights.push_back(PowerUp::Type::Mushroom);

  if (mouse_.lives() == 1) weights.push_back(PowerUp::Type::Mushroom);
  if (enemies_.size() > 5) weights.push_back(PowerUp::Type::Mushroom);

  if (mouse_.satiety() < 7) weights.push_back(PowerUp::Type::Cheese);
  if (mouse_.satiety() < 5) weights.push_back(PowerUp::Type::Cheese);
  if (mouse_.satiety() < 2) weights.push_back(PowerUp::Type::Cheese);

  if (flower_.nutrients() < 7) weights.push_back(PowerUp::Type::Leaf);
  if (flower_.nutrients() < 5) weights.push_back(PowerUp::Type::Leaf);
  if (flower_.nutrients() < 2) weights.push_back(PowerUp::Type::Leaf);

  if (flower_.water() < 7) weights.push_back(PowerUp::Type::Droplet);
  if (flower_.water() < 5) weights.push_back(PowerUp::Type::Droplet);
  if (flower_.water() < 2) weights.push_back(PowerUp::Type::Droplet);

  std::shuffle(weights.begin(), weights.end(), rand_);

  powerups_.emplace_back(weights.back(), maze_.random_pos());
}

void MazeScreen::spawn_enemy() {
  std::uniform_int_distribution<int> re(0, 1);
  enemies_.emplace_back(static_cast<Enemy::Type>(re(rand_)), maze_.random_edge_pos());
}

#include "title_roll.h"

void TitleRoll::update(unsigned int elapsed) {
  for (auto& e : entries_) e->update(elapsed);
}

void TitleRoll::draw(Graphics& graphics, int y) const {
  for (const auto& e : entries_) {
    if (y > -e->height() && y < graphics.height()) e->draw(graphics, y);
    y += e->height();
  }
}

void TitleRoll::Title::draw(Graphics& graphics, int y) const {
  text_.draw(graphics, title_, 128, y, Text::Alignment::Center);
}

void TitleRoll::NamedItem::draw(Graphics& graphics, int y) const {
  text_.draw(graphics, name_, 140, y, Text::Alignment::Center);
  sprites_.draw(graphics, frame_, 116 - 4 * name_.length(), y);
}

void TitleRoll::NamedItem::update(unsigned int elapsed) {
  timer_.update(elapsed);
  if (timer_.fired()) {
    ++frame_;
    if (frame_ > n2_) frame_ = n1_;
  }
}

TitleRoll::Description::Description(const std::string& desc) : desc_(desc) {
  lines_ = std::count(desc_.begin(), desc_.end(), '\n') + 1;
}

#include "appearing_text.h"

AppearingText::AppearingText(const std::string& text) : count_(0), text_(text) {}

/* void AppearingText::update(Audio& audio, unsigned int elapsed) { */
/*   count_ += elapsed; */
/*   if (!done() && count_ / LETTER_RATE > (count_ - elapsed) / LETTER_RATE) { */
/*     audio.play_sample("bla" + std::to_string(std::rand() % 7) + ".wav"); */
/*   } */
/* } */

void AppearingText::draw(Graphics& graphics, const Text& text, int x, int y) const {
  const int letters = count_ / kLetterRate;
  if (letters > 0) text.draw(graphics, text_.substr(0, letters), x, y);
}

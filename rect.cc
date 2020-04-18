#include "rect.h"

Rect::Rect(float x, float y, float w, float h) : x_(x), y_(y), w_(w), h_(h) {}

float Rect::top() const { return y_; }
float Rect::left() const { return x_; }
float Rect::right() const { return x_ + w_; }
float Rect::bottom() const { return y_ + h_; }

float Rect::xmid() const { return x_ + w_ / 2.0f; }
float Rect::ymid() const { return y_ + h_ / 2.0f; }

float Rect::width() const { return w_; }
float Rect::height() const { return h_; }

bool Rect::intersect(const Rect& other) const {
  return false;
}

#pragma once

#include <SDL2/SDL.h>

class Rect {
  public:
    Rect(float x, float y, float w, float h) : x_(x), y_(y), w_(w), h_(h) {}

    bool intersect(const Rect& other) const;

    float top() const { return y_; }
    float left() const { return x_; }
    float right() const { return x_ + w_; }
    float bottom() const { return y_ + h_; }

    float xmid() const { return x_ + w_ / 2.0f; }
    float ymid() const { return y_ + h_ / 2.0f; }

    float width() const { return w_; }
    float height() const { return h_; }

    SDL_Rect offset_sdl_rect(int xo, int yo) const;

  private:

    float x_, y_, w_, h_;
};

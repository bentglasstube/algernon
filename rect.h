#pragma once

class Rect {
  public:
    Rect(float x, float y, float w, float h);

    bool intersect(const Rect& other) const;

    float top() const;
    float left() const;
    float right() const;
    float bottom() const;

    float xmid() const;
    float ymid() const;

    float width() const;
    float height() const;

  private:

    float x_, y_, w_, h_;
};

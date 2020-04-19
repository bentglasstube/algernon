#pragma once

#include <bitset>
#include <random>
#include <stack>
#include <vector>

#include "graphics.h"
#include "spritemap.h"

class Maze {

  public:

    typedef std::bitset<4> Cell;
    struct Point { int x, y; };

    Maze(int width, int height);

    void draw(Graphics& graphics, int xo, int yo) const;

    void step();
    void generate() { while (!done()) { step(); } }
    bool done() const { return frontier_.empty() && breakup_ == 0; }
    bool letters_done() const { return letters_.empty(); }

    bool wall(Point p, int dir) const;

    int width() const { return width_; }
    int height() const { return height_; }

    bool straight_path(Point a, Point b) const;

    void add_title_steps();
    void open_middle();

  private:

    typedef std::vector<Point> Letter;

    int width_, height_, breakup_;
    std::vector<Cell> cells_;
    SpriteMap tiles_;

    std::vector<Letter> letters_;
    std::stack<Point> frontier_;
    std::mt19937 rand_;

    bool valid(Point p) const { return p.x >= 0 && p.x < width_ && p.y >= 0 && p.y < height_; }
    uint8_t at(Point p) const { return valid(p) ? cells_[p.y * width_ + p.x].to_ulong() : 31; }

    std::vector<Point> unvisited_neighbors(Point p) const;

    void open_wall(Point a, Point b);
    void unset(Point p, uint8_t bit) { if (valid(p)) cells_[p.y * width_ + p.x].reset(bit); }

};

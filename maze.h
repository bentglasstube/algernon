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

    Maze(int width, int height);

    void draw(Graphics& graphics, int xo, int yo) const;

    void step();
    void generate();
    bool done() const;

    bool wall(int x, int y, int dir) const;

    int width() const;
    int height() const;

  private:

    struct Point { int x, y; };

    int width_, height_, breakup_;
    std::vector<Cell> cells_;
    SpriteMap tiles_;

    std::stack<Point> frontier_;
    std::mt19937 rand_;

    bool valid(Point p) const;
    uint8_t at(Point p) const;

    std::vector<Point> unvisited_neighbors(Point p) const;

    void open_wall(Point a, Point b);
    void unset(Point p, uint8_t bit);

};

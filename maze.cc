#include "maze.h"

#include <algorithm>

#include "util.h"

Maze::Maze(int width, int height) :
  width_(width), height_(height), tiles_("tiles.png", 4, 16, 16)
{
  cells_.reserve(width * height);
  for (int i = 0; i < width * height; ++i) {
    cells_.push_back(15);
  }

  rand_.seed(Util::random_seed());
  std::uniform_int_distribution<int> rx(0, width_ - 1);
  std::uniform_int_distribution<int> ry(0, height_ - 1);

  breakup_ = width * height / 5;

  // start at a random position in the maze
  frontier_.push({rx(rand_), ry(rand_)});
}

void Maze::step() {
  while (!letters_.empty()) {
    auto& letter = letters_.back();

    if (letter.size() > 1) {
      auto a = letter.back();
      letter.pop_back();
      open_wall(a, letter.back());
      return;
    } else {
      letters_.pop_back();
    }
  }

  while (!frontier_.empty()) {
    auto p = frontier_.top();

    std::vector<Point> neighbors = unvisited_neighbors(p);

    if (neighbors.size() < 2) frontier_.pop();

    if (neighbors.size() > 0) {
      std::uniform_int_distribution<size_t> d(0, neighbors.size() - 1);

      auto n = neighbors[d(rand_)];
      open_wall(p, n);
      frontier_.push(n);
      return;
    }
  }

  if (breakup_ > 0) {
    std::uniform_int_distribution<int> rx(1, width_ - 2);
    std::uniform_int_distribution<int> ry(1, height_ - 2);
    std::uniform_int_distribution<int> rd(0, 3);

    const int x = rx(rand_);
    const int y = ry(rand_);
    const int d = rd(rand_);

    unset({x, y}, d);
    switch (d) {
      case 0: unset({x, y - 1}, 2); break;
      case 1: unset({x + 1, y}, 3); break;
      case 2: unset({x, y + 1}, 0); break;
      case 3: unset({x - 1, y}, 1); break;
    }

    --breakup_;
    return;
  }
}

bool Maze::wall(Maze::Point p, int dir) const {
  if (!valid(p)) return true;
  return cells_[p.y * width_ + p.x].test(dir);
}

void Maze::draw(Graphics& graphics, int xo, int yo) const {
  for (int y = 0; y < height_; ++y) {
    for (int x = 0; x < width_; ++x) {
      tiles_.draw(graphics, 0, 16 * x + xo, 16 * y + yo);
      tiles_.draw(graphics, at({x, y}), 16 * x + xo, 16 * y + yo);
    }
  }
}

std::vector<Maze::Point> Maze::unvisited_neighbors(Maze::Point p) const {
  std::vector<Point> neighbors;

  if (at({p.x - 1, p.y}) == 15) neighbors.push_back({p.x - 1, p.y});
  if (at({p.x + 1, p.y}) == 15) neighbors.push_back({p.x + 1, p.y});
  if (at({p.x, p.y - 1}) == 15) neighbors.push_back({p.x, p.y - 1});
  if (at({p.x, p.y + 1}) == 15) neighbors.push_back({p.x, p.y + 1});

  return neighbors;
}

void Maze::open_wall(Maze::Point a, Maze::Point b) {
  if (b.x == a.x - 1 && b.y == a.y) {
    unset(a, 3); unset(b, 1);
  } else if (b.x == a.x + 1 && b.y == a.y) {
    unset(a, 1); unset(b, 3);
  } else if (b.y == a.y - 1 && b.x == a.x) {
    unset(a, 0); unset(b, 2);
  } else if (b.y == a.y + 1 && b.x == a.x) {
    unset(a, 2); unset(b, 0);
  }
}

bool Maze::straight_path(Maze::Point a, Maze::Point b) const {
  if (a.x == b.x) {
    const int y1 = std::min(a.y, b.y);
    const int y2 = std::max(a.y, b.y);
    for (int y = y1; y < y2; ++y) {
      if (wall({a.x, y}, 2)) return false;
    }
    return true;
  } else if (a.y == b.y) {
    const int x1 = std::min(a.x, b.x);
    const int x2 = std::max(a.x, b.x);
    for (int x = x1; x < x2; ++x) {
      if (wall({x, a.y}, 1)) return false;
    }
    return true;
  } else {
    return false;
  }
}

void Maze::add_title_steps() {
  /* A */ letters_.push_back({{0,3},{1,3},{1,4},{1,5},{1,6},{0,6},{0,5},{0,4},{1,4}});
  /* l */ letters_.push_back({{2,3},{2,4},{2,5},{2,6}});
  /* g */ letters_.push_back({{4,5},{3,5},{3,4},{3,3},{4,3},{4,4},{4,5},{4,6},{3,6}});
  /* e */ letters_.push_back({{6,6},{5,6},{5,5},{6,5},{6,4},{6,3},{5,3},{5,4},{5,5}});
  /* r */ letters_.push_back({{7,6},{7,5},{7,4},{7,3},{8,3}});
  /* n */ letters_.push_back({{8,6},{8,5},{8,4},{9,4},{9,5},{9,6}});
  /* o */ letters_.push_back({{10,5},{10,4},{11,4},{11,5},{11,6},{10,6},{10,5}});
  /* n */ letters_.push_back({{13,6},{13,5},{13,4},{12,4},{12,5},{12,6}});
  /* s */ letters_.push_back({{14,6},{15,6},{15,5},{14,5},{14,4},{15,4}});
  /* f */ letters_.push_back({{3,7},{2,7},{2,8},{3,8},{2,8},{2,9},{2,10}});
  /* l */ letters_.push_back({{4,7},{4,8},{4,9},{4,10}});
  /* o */ letters_.push_back({{6,9},{6,8},{5,8},{5,9},{5,10},{6,10},{6,9}});
  /* w */ letters_.push_back({{8,8},{8,9},{8,10},{7,10},{7,9},{7,8},{7,9},{7,10},{8,10},{9,10},{9,9},{9,8}});
  /* e */ letters_.push_back({{11,8},{11,7},{10,7},{10,8},{10,9},{11,9},{11,8},{11,9},{10,9},{10,10},{11,10}});
  /* r */ letters_.push_back({{13,7},{12,7},{12,8},{12,9},{12,10}});

  std::shuffle(letters_.begin(), letters_.end(), rand_);

  frontier_.pop();
  frontier_.push({9,3});
  frontier_.push({3,9});
  frontier_.push({5,7});

  breakup_ = 0;
}

void Maze::open_middle() {
  const int cx = width_ / 2;
  const int cy = height_ / 2;

  open_wall({cx, cy}, {cx - 1, cy});
  open_wall({cx, cy}, {cx + 1, cy});
  open_wall({cx, cy}, {cx, cy - 1});
  open_wall({cx, cy}, {cx, cy + 1});
}

Maze::Point Maze::random_pos() {
  std::uniform_int_distribution<int> rx(0, width_ - 1);
  std::uniform_int_distribution<int> ry(0, height_ - 1);

  return { rx(rand_), ry(rand_) };
}

Maze::Point Maze::random_edge_pos() {
  std::uniform_int_distribution<int> rx(0, width_ - 1);
  std::uniform_int_distribution<int> ry(0, height_ - 1);
  std::uniform_int_distribution<int> rs(0, 3);

  const int side = rs(rand_);
  switch (side) {
    case 0: return { rx(rand_), 0 };
    case 1: return { width_ - 1, ry(rand_) };
    case 2: return { rx(rand_), height_ - 1 };
    case 3: return { 0, ry(rand_) };
  }

  return { -1, -1 };
}

#include "maze.h"

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

  // Calculate middle room
  const int cx = width / 2;
  const int cy = height / 2;

  // Clear all walls to middle
  unset({cx, cy}, 0);
  unset({cx, cy}, 1);
  unset({cx, cy}, 2);
  unset({cx, cy}, 3);

  // Clear walls in corners
  unset({0, 0}, 1);
  unset({0, height - 1}, 1);
  unset({width - 1, 0}, 3);
  unset({width - 1, height - 1}, 3);

  breakup_ = width * height / 5;

  // start at a random position in the maze
  frontier_.push({rx(rand_), ry(rand_)});
}

void Maze::step() {
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

  // Fix up half walls

  // Calculate middle room
  const int cx = width_ / 2;
  const int cy = height_ / 2;

  // Clear all walls to middle
  unset({cx - 1, cy}, 1);
  unset({cx + 1, cy}, 3);
  unset({cx, cy - 1}, 2);
  unset({cx, cy + 1}, 0);

  // Clear walls in corners
  unset({1, 0}, 3);
  unset({1, height_ - 1}, 3);
  unset({width_ - 2, 0}, 1);
  unset({width_ - 2, height_ - 1}, 1);

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

void Maze::generate() {
  while (!done()) {
    step();
  }
}

bool Maze::done() const {
  return frontier_.empty() && breakup_ == 0;
}

bool Maze::wall(int x, int y, int dir) const {
  if (!valid({x, y})) return true;
  return cells_[y * width_ + x].test(dir);
}

void Maze::draw(Graphics& graphics, int xo, int yo) const {
  for (int y = 0; y < height_; ++y) {
    for (int x = 0; x < width_; ++x) {
      tiles_.draw(graphics, 0, 16 * x + xo, 16 * y + yo);
      tiles_.draw(graphics, at({x, y}), 16 * x + xo, 16 * y + yo);
    }
  }
}

bool Maze::valid(Maze::Point p) const {
  return p.x >= 0 && p.x < width_ && p.y >= 0 && p.y < height_;
}

uint8_t Maze::at(Point p) const {
  return valid(p) ? cells_[p.y * width_ + p.x].to_ulong() : 31;
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

void Maze::unset(Maze::Point p, uint8_t bit) {
  if (valid(p)) cells_[p.y * width_ + p.x].reset(bit);
}

int Maze::width() const {
  return width_;
}

int Maze::height() const {
  return height_;
}

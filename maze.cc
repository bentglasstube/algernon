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
  std::uniform_int_distribution<int> rx(0, width_);
  std::uniform_int_distribution<int> ry(0, height_);

  // start at a random position in the maze
  frontier_.push({rx(rand_), ry(rand_)});
}

void Maze::step() {
  while (!done()) {
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
}

bool Maze::done() const {
  return frontier_.empty();
}

void Maze::draw(Graphics& graphics) const {
  for (int y = 0; y < height_; ++y) {
    for (int x = 0; x < width_; ++x) {
      tiles_.draw(graphics, 0, 16 * x, 16 * y);
      tiles_.draw(graphics, at({x, y}), 16 * x, 16 * y);
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

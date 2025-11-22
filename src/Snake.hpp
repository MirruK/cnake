#pragma once
#include "Common.hpp"
#include <list>


enum class SnakeDirection {
  UP,
  RIGHT,
  DOWN,
  LEFT
};

class Food {
  public:
    static Point position_bounds;
    static void set_position_bounds(Point pos);
    Point position;
    Food();
};

class Snake {
  SnakeDirection direction;
  std::list<Point> body;

  public:
    Snake();
    const std::list<Point>& get_body();
    void set_direction(SnakeDirection new_dir);
    SnakeDirection get_direction();
    void grow();
    void step();
    void reset();
};

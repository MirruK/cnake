#pragma once
#include "Common.hpp"
#include <list>


enum class SnakeDirection {
  UP,
  RIGHT,
  DOWN,
  LEFT
};

Point create_food();

class Snake {
  SnakeDirection direction;
  std::list<Point> body;

  public:
    Snake(int init_length);
    const std::list<Point>& get_body();
    void set_direction(SnakeDirection new_dir);
    SnakeDirection get_direction();
    void grow();
    void step();
    void reset();
    Point get_head();
};

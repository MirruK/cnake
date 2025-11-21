#pragma once
#include <list>

typedef struct {
  int x;
  int y;
} Point;

Point p_add(Point p1, Point p2);
Point p_sub(Point p1, Point p2);

enum class SnakeDirection {
  UP,
  RIGHT,
  DOWN,
  LEFT
};

class Food {
  static Point position_bounds;
  public:
    static void set_position_bounds(Point pos);
    Point position;
    Food();
};

class Snake {
  SnakeDirection direction;
  std::list<Point> body;

  public:
    void grow();
    void step();
};

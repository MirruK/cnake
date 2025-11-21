#include "Snake.hpp"
#include <cstdlib>
// SNek implementation

Point p_add(Point p1, Point p2) {
  return Point {p2.x + p1.x, p2.y + p1.y};
}

Point p_sub(Point p1, Point p2){
  return Point {p1.x - p2.x, p1.y - p2.y};
}

/* Must be called before creating Food-objects, eg. with {board_size.x, board_size.y} */
void Food::set_position_bounds(Point pos) {
  Food::position_bounds = pos;
}

// TODO: Use method that allows specifying random range
Food::Food(): position(std::rand(), std::rand()) {};


void Snake::grow() {
  auto len = body.size();
  Point last = body.back();            
  Point new_tail;

  if (len > 1){
    Point slast = {0,0};
    auto it = body.crbegin();
    it++;
    // UNCHECKED: unchecked dereference
    slast = *it;
    // Create a line through the two last body pieces to determine where new one is placed
    new_tail = p_add(last, p_sub(slast, last));
  }else {
    // Our poor snake has only a head (len == 1)
    switch (this->direction) {
      case SnakeDirection::UP:
        new_tail = {last.x + 0, last.y + 1};
        break;
      case SnakeDirection::RIGHT:
        new_tail = {last.x - 1, last.y + 0};
        break;
      case SnakeDirection::DOWN:
        new_tail = {last.x + 0, last.y - 1};
        break;
      case SnakeDirection::LEFT:
        new_tail = {last.x + 1, last.y + 0};
        break;
      }
  }
  body.push_back(new_tail);
}

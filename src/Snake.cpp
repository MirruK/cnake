#include "Snake.hpp"
#include "GameState.hpp"
// SNek implementation

Point Food::position_bounds = Point{0,0};

/* Must be called before creating Food-objects, eg. with {board_size.x, board_size.y} */
void Food::set_position_bounds(Point pos) {
  Food::position_bounds = pos;
}

// TODO: Use method that allows specifying random range
Food::Food(): position(100, 100) {};


// TODO: Use board_midpoint to init snake
Snake::Snake(): body(std::list<Point>({Point{25,25}, Point{25,26}, Point{25,27}, Point{25,28}})), direction(SnakeDirection::UP) {}


// void Snake::reset() {
//   this->body = std::li
  
// }

SnakeDirection Snake::get_direction() {
  return this->direction;
}

void Snake::set_direction(SnakeDirection new_dir) {
  this->direction = new_dir;
}

const std::list<Point>& Snake::get_body(){
  return this->body;
}


void Snake::grow() {
  auto len = body.size();
  Point last = body.back();            
  Point new_tail;

  if (len > 1){
    Point slast = {0,0};
    auto it = body.crbegin();
    it++;
    slast = *it;
    // Create a line through the two last body pieces to determine where new one is placed
    new_tail = last + (slast - last);
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

void Snake::step(){
  auto dir = this->direction; 
  auto new_head = Point {0,0};
  auto head = this->body.front();
  auto game_ctx = GameContext::get_instance();
  auto max_x = game_ctx->BOARD_SIZE.x;
  auto max_y = game_ctx->BOARD_SIZE.y;
  switch (dir) {
  case SnakeDirection::UP:
    new_head = head.y > 0 ? Point {head.x + 0, head.y - 1} : Point {head.x, max_y};
    this->body.emplace_front(new_head);
    this->body.pop_back();
    break;
  case SnakeDirection::RIGHT:
    new_head = head.x < max_x-1 ? Point {head.x + 1, head.y} : Point {0, head.y};
    this->body.emplace_front(new_head);
    this->body.pop_back();
    break;
  case SnakeDirection::DOWN:
    new_head = head.y < max_y-1 ? Point {head.x, head.y + 1} : Point {head.x, 0};
    this->body.emplace_front(new_head);
    this->body.pop_back();
    break;
  case SnakeDirection::LEFT:
    new_head = head.x > 0 ? Point {head.x - 1, head.y} : Point {max_x, head.y};
    this->body.emplace_front(new_head);
    this->body.pop_back();
    break;
  }
}

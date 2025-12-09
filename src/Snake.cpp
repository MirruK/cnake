#include "Snake.hpp"
#include "Common.hpp"
#include "GameState.hpp"
#include <random>
#include <vector>
// SNek implementation

Point create_food() {
  auto game_ctx = GameContext::get_instance();

  Point bs = game_ctx->BOARD_SIZE;
  // Static to not recreate rng object every call
  static std::mt19937 rng(std::random_device{}()); 
  std::uniform_real_distribution<float> dist(0.0f, 1.0f);

  auto r1 = dist(rng);
  auto r2 = dist(rng);
  return Point{(int)((float)bs.x * r1), (int)((float)bs.y * r2)};
};

Point Snake::get_head() {
  return this->body.front();
}


Snake::Snake(int init_length): direction(SnakeDirection::UP) {
  auto game_ctx = GameContext::get_instance();

  Point bs = game_ctx->BOARD_SIZE;
  Point midpoint = bs / 2;
  std::vector<Point> v = {midpoint};
  if (init_length > 0) {
  v.reserve(init_length);
    for (int i = 0; i < init_length - 1; i++){
      v.push_back(v[i] + Point{0, 1} );

    }
  }
  this->body =  std::list<Point>(v.begin(), v.end());
}

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
    new_head = head.y > 0 ? Point {head.x + 0, head.y - 1} : Point {head.x, max_y-1};
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
    new_head = head.x > 0 ? Point {head.x - 1, head.y} : Point {max_x-1, head.y};
    this->body.emplace_front(new_head);
    this->body.pop_back();
    break;
  }
}

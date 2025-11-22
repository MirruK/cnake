#include "Common.hpp"
#include <exception>

Point operator+(Point p1, Point p2){
  return Point {p1.x + p2.x, p1.y + p2.y};
}

Point operator-(Point p1, Point p2){
  return Point {p1.x - p2.x, p1.y - p2.y};
}

Point operator*(Point p, int s) {
  return Point {p.x * s, p.y * s};
}

Point operator/(Point p1, Point p2) {
  if (p2.x == 0 | p2.y == 0) throw std::exception();
  return Point {p1.x / p2.x, p1.y / p2.y };
}

bool operator==(Point p1, Point p2) {
  if (p1.x == p2.x && p1.y == p2.y){
    return true;
  }
  return false;
}

std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << '{' << p.x << ',' << ' ' << p.y << '}';
    return os;
}

int p_min(Point p){
  return p.x <= p.y ? p.x : p.y;
}

#pragma once
#include <ostream>

typedef struct {
  int x;
  int y;
} Point;

Point operator+(Point p1, Point p2);
Point operator-(Point p1, Point p2);
Point operator*(Point p, int s);
Point operator/(Point p1, Point p2);
bool operator==(Point p1, Point p2);
std::ostream& operator<<(std::ostream& os, const Point&);
int p_min(Point p);


#pragma once

#include <cmath>

class BlockState;

class Point{
public:
    int x, y;
    Point(int x = 0, int y = 0);
    bool operator==(Point a);
    bool operator< (const Point a) const;
    Point operator+(Point a);
    Point operator-(Point a);
    Point operator-();
    operator BlockState();
    double dist(Point b);
};
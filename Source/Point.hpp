#pragma once

class BlockState;

class Point{
public:
    int x, y;
    Point(int x = 0, int y = 0);
    Point(Point&& p);
    Point(const Point& p);
    Point operator=(Point p);
    bool operator==(Point a);
    bool operator< (const Point a) const;
    Point operator+(Point a) const;
    Point operator-(Point a) const;
    Point operator-() const;
    operator BlockState() const;
    double dist(Point b) const;
};
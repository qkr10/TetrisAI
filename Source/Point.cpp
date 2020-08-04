#include "Point.hpp"
#include "BlockState.hpp"
#include <cmath>

Point::Point(int x, int y) : x(x), y(y){}
Point::Point(Point&& p) : x(p.x), y(p.y){}
Point::Point(const Point& p) : x(p.x), y(p.y){}
Point Point::operator=(Point p)
{
    x = p.x; y = p.y;
    return *this;
}
bool Point::operator==(Point a)
{
    return a.x == x && a.y == y;
}
bool Point::operator< (const Point a) const
{
    return x < a.x;
}
Point Point::operator+(Point a) const
{
    return Point(a.x + x, a.y + y);
}
Point Point::operator-(Point a) const
{
    return Point(x - a.x, y - a.y);
}
Point Point::operator-() const
{
    return Point(0, 0) - *this;
}
Point::operator BlockState() const
{
    return BlockState(*this, 0, 0);
}
double Point::dist(Point b) const
{
    int x = this->x - b.x;
    int y = this->y - b.y;
    return sqrt(x * x + y * y);
}
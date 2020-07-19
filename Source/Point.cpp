#include "Point.hpp"

Point::Point(){x = y = 0;}
Point::Point(int x, int y) : x(x), y(y){}
bool Point::operator==(Point a)
{
    return a.x == x && a.y == y;
}
Point Point::operator+(Point a)
{
    return Point(a.x + x, a.y + y);
}
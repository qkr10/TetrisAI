#include "Point.hpp"
#include "BlockState.hpp"

Point::Point(int x, int y) : x(x), y(y){}
bool Point::operator==(Point a)
{
    return a.x == x && a.y == y;
}
bool Point::operator< (const Point a) const
{
    return x < a.x;
}
Point Point::operator+(Point a)
{
    return Point(a.x + x, a.y + y);
}
Point Point::operator-(Point a)
{
    return Point(x - a.x, y - a.y);
}
Point Point::operator-()
{
    return Point(0, 0) - *this;
}
Point::operator BlockState()
{
    return BlockState(*this, 0, 0);
}
double Point::dist(Point b)
{
    int x = this->x - b.x;
    int y = this->y - b.y;
    return sqrt(x * x + y * y);
}
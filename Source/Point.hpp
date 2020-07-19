class Point{
public:
    int x, y;
    Point();
    Point(int x, int y);
    bool operator==(Point a);
    Point operator+(Point a);
}
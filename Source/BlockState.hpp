#pragma once

#include "Point.hpp"
#include "Tetris.hpp"
#include <tuple>

using iiii = std::tuple<int&, int&, int&, int&>;

class BlockState : public Point{
private:
	static Point spinCenter[10];
	static Point shape[7][4][4];
public:
    int rot;
    int index;

    BlockState(Point p = {0, 0}, int r = 0, int i = 0);

    BlockState operator=(const BlockState& b);
    BlockState operator=(const Point& p);
    BlockState operator+(const BlockState& b) const;
    BlockState operator-(const BlockState& b) const;
    bool operator< (const BlockState b) const;
    bool operator== (const BlockState b) const;
    operator iiii();

    bool CheckBoardBoundary(Point p);
    // 벽돌의 이동/회전 가능성 조사(O : EMPTY, X : BRICK)
    // x, y, b, r = 블럭상태(위치, 블럭종류, 회전한횟수)
    // 회전이 불가능할때는 블럭이 이동할수 있으므로 GetAroundSpin써야함
    int GetAround(int (* board)[BH + 2]);
    // 벽돌의 이동/회전 가능성 조사(O : EMPTY, X : BRICK)
    // x, y, b, r = 처음 블럭상태(위치, 블럭종류, 회전한횟수)
    // retx, rety = 움직인 후 블럭 위치
    int GetAroundSpin(int (* board)[BH + 2], Point& ret);
    int GetAroundSpinRev(int (* board)[BH + 2], Point& ret);

    Point GetShape(int i);
    Point GetPos(int i);
    Point GetBoardPos(int i);
    static Point GetHoldPos(int i, int hBrick);
    static Point GetNextPos(int i, int nBrick);
};
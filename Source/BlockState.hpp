#pragma once

#include "Point.hpp"

class BlockState{
public:
    Point pos;
    int rot;
    int index;
    int& x = pos.x;
    int& y = pos.y;

    BlockState(Point p = {0, 0}, int r = 0, int i = 0);

    BlockState operator=(const BlockState& b);
    BlockState operator+(const BlockState& b);
    bool operator< (const BlockState b) const;

    Point GetPos(int i);
    // 벽돌의 이동/회전 가능성 조사(O : EMPTY, X : BRICK)
    // x, y, b, r = 블럭상태(위치, 블럭종류, 회전한횟수)
    // 회전이 불가능할때는 블럭이 이동할수 있으므로 GetAroundSpin써야함
    int GetAround();
    // 벽돌의 이동/회전 가능성 조사(O : EMPTY, X : BRICK)
    // x, y, b, r = 처음 블럭상태(위치, 블럭종류, 회전한횟수)
    // retx, rety = 움직인 후 블럭 위치
    int GetAroundSpin(Point& ret);
    int GetAroundSpinRev(Point& ret);
};
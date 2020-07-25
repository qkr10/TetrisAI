#include "Shape.hpp"
#include "BlockState.hpp"
#include "Tetris.hpp"

extern BlockState curState;

Point Shape::shape[7][4][4] = {
	{ {{0,0},{1,0},{2,0},{-1,0}}, {{0,0},{0,1},{0,-1},{0,-2}}, {{0,0},{1,0},{2,0},{-1,0}}, {{0,0},{0,1},{0,-1},{0,-2}} },
	{ {{0,0},{1,0},{0,1},{1,1}}, {{0,0},{1,0},{0,1},{1,1}}, {{0,0},{1,0},{0,1},{1,1}}, {{0,0},{1,0},{0,1},{1,1}} },
	{ {{0,0},{-1,0},{0,-1},{1,-1}}, {{0,0},{0,1},{-1,0},{-1,-1}}, {{0,0},{-1,0},{0,-1},{1,-1}}, {{0,0},{0,1},{-1,0},{-1,-1}} },
	{ {{0,0},{-1,-1},{0,-1},{1,0}}, {{0,0},{-1,0},{-1,1},{0,-1}}, {{0,0},{-1,-1},{0,-1},{1,0}}, {{0,0},{-1,0},{-1,1},{0,-1}} },
	{ {{0,0},{-1,0},{1,0},{-1,-1}}, {{0,0},{0,-1},{0,1},{-1,1}}, {{0,0},{-1,0},{1,0},{1,1}}, {{0,0},{0,-1},{0,1},{1,-1}} },
	{ {{0,0},{1,0},{-1,0},{1,-1}}, {{0,0},{0,1},{0,-1},{-1,-1}}, {{0,0},{1,0},{-1,0},{-1,1}}, {{0,0},{0,-1},{0,1},{1,1}} },
	{ {{0,0},{-1,0},{1,0},{0,1}}, {{0,0},{0,-1},{0,1},{1,0}}, {{0,0},{-1,0},{1,0},{0,-1}}, {{0,0},{-1,0},{0,-1},{0,1}} }
};


Point Shape::GetShape(int i)
{
    return shape[curState.index][curState.rot][i];
}

Point Shape::GetPos(int i)
{
    return curState.GetPos(i);
}

Point Shape::GetBoardPos(int i)
{
    int xCor = BX + (GetShape(i).x + curState.x) * 2;
    int yCor = BY + GetShape(i).y + curState.y;
    return Point(xCor, yCor);
}

Point Shape::GetHoldPos(int i, int hBrick)
{
    int xCor = HOLD_X + (shape[hBrick][0][i].x) * 2;
    int yCor = HOLD_Y + shape[hBrick][0][i].y;
    return Point(xCor, yCor);
}

Point Shape::GetNextPos(int i, int nBrick)
{
    int xCor = NEXT_X + (shape[nBrick][0][i].x) * 2;
    int yCor = NEXT_Y + shape[nBrick][0][i].y;
    return Point(xCor, yCor);
}
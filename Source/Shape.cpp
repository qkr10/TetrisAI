#include "Shape.hpp"
#include "BlockState.hpp"
#include "Tetris.hpp"

extern BlockState curState;
extern int nbrick;
extern int hbrick;

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

Point Shape::GetHoldPos(int i)
{
    int xCor = HOLD_X + (shape[hbrick][0][i].x) * 2;
    int yCor = HOLD_Y + shape[hbrick][0][i].y;
    return Point(xCor, yCor);
}

Point Shape::GetNextPos(int i)
{
    int xCor = NEXT_X + (shape[nbrick][0][i].x) * 2;
    int yCor = NEXT_Y + shape[nbrick][0][i].y;
    return Point(xCor, yCor);
}
#include "BlockState.hpp"
#include "Tetris.hpp"
#include <algorithm>

Point BlockState::spinCenter[10] = {
	{0, 0},{1, 0},{-1, 0},
	{0, 1},{1, 1},{-1, 1},
	{0, 2},{1, 2},{-1, 2},
	{0, -1}
};

Point BlockState::shape[7][4][4] = {
	{ {{0,0},{1,0},{2,0},{-1,0}}, {{0,0},{0,1},{0,-1},{0,-2}}, {{0,0},{1,0},{2,0},{-1,0}}, {{0,0},{0,1},{0,-1},{0,-2}} },
	{ {{0,0},{1,0},{0,-1},{1,-1}}, {{0,0},{1,0},{0,-1},{1,-1}}, {{0,0},{1,0},{0,-1},{1,-1}}, {{0,0},{1,0},{0,-1},{1,-1}} },
	{ {{0,0},{-1,0},{0,-1},{1,-1}}, {{0,0},{0,1},{-1,0},{-1,-1}}, {{0,0},{-1,0},{0,-1},{1,-1}}, {{0,0},{0,1},{-1,0},{-1,-1}} },
	{ {{0,0},{-1,-1},{0,-1},{1,0}}, {{0,0},{-1,0},{-1,1},{0,-1}}, {{0,0},{-1,-1},{0,-1},{1,0}}, {{0,0},{-1,0},{-1,1},{0,-1}} },
	{ {{0,0},{-1,0},{1,0},{-1,-1}}, {{0,0},{0,-1},{0,1},{-1,1}}, {{0,0},{-1,0},{1,0},{1,1}}, {{0,0},{0,-1},{0,1},{1,-1}} },
	{ {{0,0},{1,0},{-1,0},{1,-1}}, {{0,0},{0,1},{0,-1},{-1,-1}}, {{0,0},{1,0},{-1,0},{-1,1}}, {{0,0},{0,-1},{0,1},{1,1}} },
	{ {{0,0},{-1,0},{1,0},{0,1}}, {{0,0},{0,-1},{0,1},{1,0}}, {{0,0},{-1,0},{1,0},{0,-1}}, {{0,0},{-1,0},{0,-1},{0,1}} }
};

BlockState::BlockState(Point p, int r, int i) : Point(p), rot(r), index(i) {}

BlockState BlockState::operator=(const BlockState& b)
{
	*this = (Point)b;
	rot = b.rot;
	index = b.index;
	return *this;
}

BlockState BlockState::operator=(const Point& p)
{
	x = p.x; y = p.y;
	return *this;
}

BlockState BlockState::operator+(const BlockState& b) const
{
    BlockState newState(*this);
	newState = (Point)newState + b;
	newState.rot = (newState.rot + b.rot + 4) % 4;
	newState.index = newState.index + b.index;
	return newState;
}

BlockState BlockState::operator-(const BlockState& b) const
{
    BlockState newState(*this);
	newState = (Point)newState - b;
	newState.rot = (newState.rot - b.rot + 4) % 4;
	newState.index = newState.index - b.index;
	return newState;
}

bool BlockState::operator< (const BlockState b) const
{
	return x < b.x;
}

bool BlockState::operator== (const BlockState b) const
{
	return rot == b.rot && index == b.index && (Point)*this == b;
}

BlockState::operator iiii()
{
	return std::tie(x, y, rot, index);
}

bool BlockState::CheckBoardBoundary(Point p)
{
	return 0 < p.x && p.x <= BW && 0 < p.y && p.y <= BH;
}

int BlockState::GetAround(int (* board)[BH + 2])
{
	int k = EMPTY;
	for (int i = 0; i < 4; i++){
		if (!CheckBoardBoundary(GetPos(i))) return WALL;
		k = std::max(k, board[GetPos(i).x][GetPos(i).y]);
	}
	return k;
}

int BlockState::GetAroundSpin(int (* board)[BH + 2], Point& ret)
{
	for (int j = 0; j < 10; j++) {
        BlockState newPos = *this + (BlockState)spinCenter[j];
		if (newPos.GetAround(board) == EMPTY) {
            ret = newPos;
			return EMPTY;
		}
	}
	return BRICK;
}

int BlockState::GetAroundSpinRev(int (* board)[BH + 2], Point& ret)
{
	for (int j = 0; j < 10; j++) {
        BlockState newPos = *this - (BlockState)spinCenter[j];
		if (newPos.GetAroundSpin(board, ret) == EMPTY && ret == *this) {
            ret = newPos;
			return EMPTY;
		}
	}
	return BRICK;
}

Point BlockState::GetShape(int i)
{
	return shape[index][rot][i];
}

Point BlockState::GetPos(int i)
{
	return (Point)*this + shape[index][rot][i];
}

Point BlockState::GetBoardPos(int i)
{
	return Point(BX + (GetPos(i).x * 2), BY + GetPos(i).y);
}

Point BlockState::GetHoldPos(int i, int hBrick)
{
	BlockState temp = BlockState({0, 0}, 0, hBrick);
	return Point(HOLD_X + (temp.GetShape(i).x * 2), HOLD_Y + temp.GetShape(i).y);
}

Point BlockState::GetNextPos(int i, int nBrick)
{
	BlockState temp = BlockState({0, 0}, 0, nBrick);
	return Point(NEXT_X + (temp.GetShape(i).x * 2), NEXT_Y + temp.GetShape(i).y);
}
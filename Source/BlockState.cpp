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
	{ {{0,0},{1,0},{0,1},{1,1}}, {{0,0},{1,0},{0,1},{1,1}}, {{0,0},{1,0},{0,1},{1,1}}, {{0,0},{1,0},{0,1},{1,1}} },
	{ {{0,0},{-1,0},{0,-1},{1,-1}}, {{0,0},{0,1},{-1,0},{-1,-1}}, {{0,0},{-1,0},{0,-1},{1,-1}}, {{0,0},{0,1},{-1,0},{-1,-1}} },
	{ {{0,0},{-1,-1},{0,-1},{1,0}}, {{0,0},{-1,0},{-1,1},{0,-1}}, {{0,0},{-1,-1},{0,-1},{1,0}}, {{0,0},{-1,0},{-1,1},{0,-1}} },
	{ {{0,0},{-1,0},{1,0},{-1,-1}}, {{0,0},{0,-1},{0,1},{-1,1}}, {{0,0},{-1,0},{1,0},{1,1}}, {{0,0},{0,-1},{0,1},{1,-1}} },
	{ {{0,0},{1,0},{-1,0},{1,-1}}, {{0,0},{0,1},{0,-1},{-1,-1}}, {{0,0},{1,0},{-1,0},{-1,1}}, {{0,0},{0,-1},{0,1},{1,1}} },
	{ {{0,0},{-1,0},{1,0},{0,1}}, {{0,0},{0,-1},{0,1},{1,0}}, {{0,0},{-1,0},{1,0},{0,-1}}, {{0,0},{-1,0},{0,-1},{0,1}} }
};

BlockState::BlockState(Point p, int r, int i) : pos(p), rot(r), index(i) {}

BlockState BlockState::operator=(const BlockState& b)
{
	pos = b.pos;
	rot = b.rot;
	index = b.index;
	return *this;
}

BlockState BlockState::operator+(const BlockState& b)
{
    BlockState newState(*this);
	newState.pos = newState.pos + b.pos;
	newState.rot = (newState.rot + b.rot + 4) % 4;
	newState.index = newState.index + b.index;
	return newState;
}

BlockState BlockState::operator-(const BlockState& b)
{
    BlockState newState(*this);
	newState.pos = newState.pos - b.pos;
	newState.rot = (newState.rot - b.rot + 4) % 4;
	newState.index = newState.index - b.index;
	return newState;
}

bool BlockState::operator< (const BlockState b) const
{
	return x < b.x;
}

bool BlockState::CheckBoardBoundary(Point p)
{
	return 0 < p.x && p.x < 11 && 0 < p.y && p.y < 21;
}

int BlockState::GetAround(int (* board)[BH + 2])
{
	int k = EMPTY;
	for (int i = 0; i < 4; i++){
		if (CheckBoardBoundary(GetPos(i))) continue;
		k = std::max(k, board[GetPos(i).x][GetPos(i).y]);
	}
	return k;
}

int BlockState::GetAroundSpin(int (* board)[BH + 2], Point& ret)
{
	for (int j = 0; j < 10; j++) {
        BlockState newPos = *this + (BlockState)spinCenter[j];
		if (newPos.GetAround(board) == EMPTY) {
            ret = newPos.pos;
			return EMPTY;
		}
	}
	return !EMPTY;
}

int BlockState::GetAroundSpinRev(int (* board)[BH + 2], Point& ret)
{
	for (int j = 0; j < 10; j++) {
        BlockState newPos = *this + (BlockState)-spinCenter[j];
		if (newPos.GetAround(board) == EMPTY) {
            ret = newPos.pos;
			return EMPTY;
		}
	}
	return !EMPTY;
}

Point BlockState::GetShape(int i)
{
	return shape[index][rot][i];
}

Point BlockState::GetPos(int i)
{
	return pos + shape[index][rot][i];
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
#include <algorithm>
#include "BlockState.hpp"

#undef max

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

int BlockState::GetAround(vvi &board)
{
	int k = EMPTY;
	for (int i = 0; i < 4; i++){
		if (!GetPos(i).IsIn(Point(0, 0), TetrisVariables::boardSize + Point(1, 1)))
			return WALL;
		k = std::max(k, board[GetPos(i).x][GetPos(i).y]);
	}
	return k;
}

int BlockState::GetAroundSpin(vvi &board, Point& ret)
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

int BlockState::GetAroundSpinRev(vvi &board, Point& ret)
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
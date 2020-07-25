#include "BlockState.hpp"
#include "Shape.hpp"
#include "Tetris.hpp"
#include <algorithm>

extern int board[BW + 2][BH + 2];
extern Point spinCenter[10];

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

bool BlockState::operator< (const BlockState b) const
{
	return x < b.x;
}

Point BlockState::GetPos(int i)
{
	return pos + Shape::shape[index][rot][i];
}

int BlockState::GetAround()
{
	int k = EMPTY;
	for (int i = 0; i < 4; i++)
		k = std::max(k, board[GetPos(i).x][GetPos(i).y]);
	return k;
}

int BlockState::GetAroundSpin(Point& ret)
{
	for (int j = 0; j < 10; j++) {
        BlockState newPos = *this + (BlockState)spinCenter[j];
		if (newPos.GetAround() == EMPTY) {
            ret = newPos.pos;
			return EMPTY;
		}
	}
	return !EMPTY;
}

int BlockState::GetAroundSpinRev(Point& ret)
{
	for (int j = 0; j < 10; j++) {
        BlockState newPos = *this + (BlockState)-spinCenter[j];
		if (newPos.GetAround() == EMPTY) {
            ret = newPos.pos;
			return EMPTY;
		}
	}
	return !EMPTY;
}
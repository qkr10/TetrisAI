#pragma once

#include "Point.hpp"

class Shape{
public:
	//Shape[벽돌모양][벽돌의회전][x,y좌표값]
	//구조체 3차원 배열으로 벽돌모양을 표현한다
	static Point shape[7][4][4];

	//currentshape
	static Point GetShape(int i);
	//currentshape + currentpos
	static Point GetPos(int i);
	//currentshape + currentpos + boardpos
	static Point GetBoardPos(int i);
	//holdshape + holdpos
	static Point GetHoldPos(int i);
	//nextshape + nextpos
	static Point GetNextPos(int i);
};
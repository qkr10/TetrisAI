#pragma once

#include "Windows.h"
#include <ctime>
#include "Point.hpp"

enum BOARD_STATE { EMPTY, BRICK, WALL };

class TetrisVariables
{
public:
    static Point startCor;
    static Point boardSize;
};
﻿#pragma once

#include <vector>
#include "BlockState.hpp"

using vi = std::vector<int>;
using vvi = std::vector<vi>;
using vvvi = std::vector<vvi>;

using vp = std::vector<Point>;
using vvp = std::vector<vp>;

using BlocksState = std::vector<BlockState>;

class TetrisAI
{
private:
    bool GetPathFromVisit(vvvi& visit, BlockState startPos, BlockState dest);
    bool CalcPath(BlockState state, int mode = 0);
    BlockState CalcBestState(); //못찾을시 {{-1, -1}, -1} 반환
    void CalcSteps();

    int currentStepIndex = 0;
    BlocksState steps;
public:
    int board[BW+2][BH+2]; //현재 보드(BW*BH)(enum BOARD_STATE)(AI가 이동시킬 블럭은 제거)
    vi blocksShapeIndex; //이동시킬 블럭의 인덱스를 위치0번에 저장
    BlockState GetNextStep();
};
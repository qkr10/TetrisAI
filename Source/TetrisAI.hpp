#pragma once

#include <algorithm>
#include <cmath>
#include <queue>
#include <tuple>
#include <vector>
#include <cstring>
#include "Tetris.hpp"

using vi = std::vector<int>;
using vvi = std::vector<vi>;
using vvvi = std::vector<vvi>;

using vp = std::vector<Point>;
using vvp = std::vector<vp>;

struct BlockState{
    Point BlockPos;
    int BlockRot;
    int BlockIndex;
};
using BlocksState = std::vector<BlockState>;

class TetrisAI
{
private:
    double dist(Point a, Point b);
    int GetAroundSpinRev(BlockState a, Point& ret);

    bool GetPathFromArr(vvvi& arr, BlockState startPos, BlockState dest);
    bool IsReachable(BlockState state);
    BlockState CalcBestState(); //못찾을시 {{-1, -1}, -1} 반환
    void CalcSteps();

    int currentStepIndex = 0;
    BlocksState steps;
public:
    vvi board; //현재 보드(BW*BH)(enum BOARD_STATE)(AI가 이동시킬 블럭은 제거)
    vi blocksShapeIndex; //이동시킬 블럭의 인덱스를 위치0번에 저장
    TetrisAI();
    BlockState GetNextStep();
};
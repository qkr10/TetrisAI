#pragma once

#include <vector>
#include "BlockState.hpp"
#include "TetrisVariables.hpp"

using vi = std::vector<int>;
using vvi = std::vector<vi>;
using vvvi = std::vector<vvi>;

using vp = std::vector<Point>;
using vvp = std::vector<vp>;

using BlocksState = std::vector<BlockState>;

class TetrisAI
{
private:
    static BlocksState stepKinds;

    bool GetPathFromVisit(vvvi& visit, BlockState& startPos, BlockState& dest);
    bool CalcPath(BlockState& state, int mode);
    BlockState CalcBestState(); //못찾을시 {{-1, -1}, -1, -1} 반환
    void CalcSteps();
public:
    int currentStepIndex = 0;
    BlocksState steps;
    vvi board; //현재 보드(BW*BH)(enum BOARD_STATE)(AI가 이동시킬 블럭은 제거)
    //int board[BW+2][BH+2]; //현재 보드(BW*BH)(enum BOARD_STATE)(AI가 이동시킬 블럭은 제거)
    vi blocksShapeIndex; //이동시킬 블럭의 인덱스를 위치0번에 저장
    BlockState GetNextStep();
};
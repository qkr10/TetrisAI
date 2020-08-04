#include "TetrisAI.hpp"

#include <algorithm>
#include <cmath>
#include <queue>
#include <tuple>
#include <fstream>
#include <iomanip>
#include "Tetris.hpp"
#include "Output.hpp"

BlocksState TetrisAI::stepKinds = BlocksState({
    BlockState({0,  1}, 0, 0), 
    BlockState({-1, 0}, 0, 0), 
    BlockState({1,  0}, 0, 0), 
    BlockState({0,  0}, 1, 0), 
    BlockState({0,  0}, -1, 0)
});

/***********************/
/*      함수 정의      */
/***********************/

bool TetrisAI::GetPathFromVisit(vvvi& visit, BlockState& start, BlockState& dest)
{
    Output::consoleOut << "get path from visit, start : " << start << std::endl;
    auto [x, y, rot, index] = (iiii)start;

    if (visit[x][y][rot] == 0) {
        steps.push_back(BlockState({0, 0}, 0, -2));
        return true;
    }

    for (int i = 0; i < stepKinds.size(); i++){
        BlockState newState = start + stepKinds[i];
        Point res = newState;
        int testRet = stepKinds[i].rot == 0 ? newState.GetAround(board) : newState.GetAroundSpin(board, res);
        newState = res;
        auto& newStateVisit = visit[newState.x][newState.y][newState.rot];
        if (testRet == BOARD_STATE::EMPTY &&
            (newStateVisit == visit[x][y][rot] - 1))
        {
            auto temp = newStateVisit;
            steps.push_back(newState);
            if (GetPathFromVisit(visit, newState, dest))
                return true;
            steps.pop_back();
        }
    }
    
    return false;
}

bool TetrisAI::CalcPath(BlockState& dest, int mode)
{
    vvvi visitBoard(BW + 2, vvi(BH + 2, vi(4, -1)));
    visitBoard[dest.x][dest.y][dest.rot] = 0;

    using DSPair = std::pair<double, BlockState>;
    using PQ = std::priority_queue<DSPair, std::vector<DSPair>, std::greater<DSPair>>;
    PQ pq;
    BlockState start({START_X, START_Y}, 0, blocksShapeIndex[0]);
    pq.push(DSPair(dest.dist(start), dest));

    while (!pq.empty()){
        DSPair topElement = pq.top();
        auto [dis, state] = topElement;
        auto [x, y, rot, index] = (iiii)state;
        pq.pop();

        if (state == start){
            if (mode == 1) {
                steps.clear();
                if (!GetPathFromVisit(visitBoard, start, dest)) {
                    Output::consoleOut << "failed to get path from visit" << std::endl;
                    Output::consoleOut << "\tdest : " << dest << std::endl;
                    for (int ir = 0; ir < 4; ir++) {
                        for (int iy = 0; iy < BH+2; iy++) {
                            Output::consoleOut << '\t';
                            for (int ix = 0; ix < BW+2; ix++)
                                Output::consoleOut << std::setfill(' ') << std::setw(2) << visitBoard[ix][iy][ir];
                            Output::consoleOut << std::endl;
                        }
                        Output::consoleOut << std::endl;
                    }
                }
            }
            return true;
        }

        for (int i = 0; i < stepKinds.size(); i++){
            BlockState newState = state - stepKinds[i];
            Point res = newState;
            int ret = stepKinds[i].rot == 0 ? newState.GetAround(board) : newState.GetAroundSpinRev(board, res);
            newState = res;
            auto& newStateVisit = visitBoard[newState.x][newState.y][newState.rot];
            if (ret == BOARD_STATE::EMPTY &&
                (newStateVisit == -1 || newStateVisit > visitBoard[x][y][rot]))
            {
                newStateVisit = visitBoard[x][y][rot] + 1;
                pq.push(DSPair(newState.dist(start), newState));
            }
        }
    }
    return false;
}

BlockState TetrisAI::CalcBestState()
{
    //후보군(BlocksState states) 정하기
    BlocksState states;
    for (int y = BH; y > 0; y--){
        for (int x = 1; x <= BW; x++){
            if (board[x][y] == BOARD_STATE::BRICK)
                continue;

            for (int rot = 0; rot < 4; rot++){
                BlockState state = BlockState({x, y}, rot, blocksShapeIndex[0]);
                BlockState newState = state + BlockState({0, 1}, 0, 0);
                if (state.GetAround(board) == BOARD_STATE::EMPTY &&
                    newState.GetAround(board) != BOARD_STATE::EMPTY) {
                    if (CalcPath(state, 0)){
                        states.push_back(state);
                    }
                }
            }
        }
    }
    if (states.size() == 0)
        return {{-1, -1}, -1, -1};

    //후보군을 더 나은것 우선으로 정렬
    Point start(START_X, START_Y);
    sort(states.begin(), states.end(), [&](BlockState a, BlockState b)
        {
            return start.dist(a) > start.dist(b);
        });

    return states[0];
}

void TetrisAI::CalcSteps()
{
    BlockState dest = CalcBestState();
    if (dest.index == -1){
        return;
    }
    Output::consoleOut << "dest : " << dest << std::endl;
    CalcPath(dest, 1);
    return;
}

BlockState TetrisAI::GetNextStep(){
    if (steps.size() <= currentStepIndex){
        CalcSteps();
        Output::consoleOut << "steps size : " << steps.size() << std::endl;
        for (auto var : steps)
            Output::consoleOut << "\tstep : " << var << std::endl;
        currentStepIndex = 0;
    }
    return steps[currentStepIndex++];
}
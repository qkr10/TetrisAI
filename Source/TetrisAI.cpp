#include "TetrisAI.hpp"

#include <algorithm>
#include <cmath>
#include <queue>
#include <tuple>
#include <cstring>
#include "Tetris.hpp"

vp posSteps({
    {0, -1}, {-1, 0}, {1, 0}
});
vvvi visitBoard;

/***********************/
/*      함수 정의      */
/***********************/

TetrisAI::TetrisAI()
{
    board = vvi(BW, vi(BH));
}

bool TetrisAI::GetPathFromVisit(vvvi& visit, BlockState start, BlockState dest)
{
    auto [pos, rot, index, x, y] = start;

    for (int i = 0; i < posSteps.size(); i++){
        Point p = pos + posSteps[i];
        BlockState s = {p, rot, index};
        if (s.GetAround() == BOARD_STATE::EMPTY &&
            visit[p.x][p.y][rot] == visit[x][y][rot] - 1)
        {
            steps.push_back({p, rot, index});
            if (GetPathFromVisit(visit, {p, rot, index}, dest))
                return true;
            steps.pop_back();
        }
    }

    for (int r = 0; r < 4; r++){
        if (rot == r)
            continue;
        BlockState a = start;
        a.rot = r;
        Point res;
        if (a.GetAroundSpinRev(res) == BOARD_STATE::EMPTY &&
            visit[res.x][res.y][r] == visit[x][y][rot] - 1)
        {
            a.pos = res;
            steps.push_back(a);
            if (GetPathFromVisit(visit, a, dest))
                return true;
            steps.pop_back();
        }
    }
    
    return false;
}

bool TetrisAI::CalcPath(BlockState dest, int mode)
{
    visitBoard.clear();
    visitBoard.assign(BW, vvi(BH, vi(4, -1)));

    auto [pos, rot, index, x, y] = dest;
    Point startPos = {START_X, START_Y};

    using DSPair = std::pair<double, BlockState>;
    using PQ = std::priority_queue<DSPair, std::vector<DSPair>, std::greater<DSPair>>;
    PQ pq;

    visitBoard[x][y][rot] = 0;
    pq.push(DSPair(pos.dist(startPos), dest));
    while (!pq.empty()){
        auto [dis, state] = pq.top();
        auto [pos, rot, index, x, y] = state;
        pq.pop();

        if (pos == startPos){
            if (mode != 0)
                GetPathFromVisit(visitBoard, {startPos, 0, index}, dest);
            return true;
        }

        for (int i = 0; i < posSteps.size(); i++){
            Point p = pos + posSteps[i];
            BlockState s = {p, rot, index};
            if (s.GetAround() == BOARD_STATE::EMPTY &&
                (visitBoard[p.x][p.y][rot] == -1 || visitBoard[p.x][p.y][rot] > visitBoard[x][y][rot]))
            {
                visitBoard[p.x][p.y][rot] = visitBoard[x][y][rot] + 1;
                pq.push(DSPair(p.dist(startPos), s));
            }
        }

        for (int r = 0; r < 4; r++){
            if (state.rot == r)
                continue;
            BlockState a = state;
            a.rot = r;
            Point res;
            if (a.GetAroundSpinRev(res) == BOARD_STATE::EMPTY &&
                (visitBoard[res.x][res.y][r] == -1 || visitBoard[res.x][res.y][r] > visitBoard[x][y][rot]))
            {
                visitBoard[res.x][res.y][r] = visitBoard[x][y][rot] + 1;
                a.pos = res;
                pq.push(DSPair(res.dist(startPos), a));
            }
        }
    }
    return false;
}

BlockState TetrisAI::CalcBestState()
{
    BlockState state = {{0, 0}, 0, blocksShapeIndex[0]};
    auto& [pos, rot, index, x, y] = state;
    BlocksState states;
    for (; y >= 0; y--){
        for (; x < 10; x++){
            if (board[x][y] == BOARD_STATE::BRICK)
                continue;
            for (; rot < 4; rot++){
                BlockState a = {{x, y}, rot, index};
                BlockState b = {{x, y + 1}, rot, index};
                if (a.GetAround() == BOARD_STATE::EMPTY &&
                    b.GetAround() >= BOARD_STATE::BRICK){
                    if (CalcPath(state)){
                        states.push_back(state);
                    }
                }
            }
        }
    }
    if (states.size() == 0)
        return {{-1, -1}, -1, -1};

    Point start(START_X, START_Y);
    sort(states.begin(), states.end(), [&](BlockState a, BlockState b)
        {
            return start.dist(a.pos) > start.dist(b.pos);
        });

    return states[0];
}

void TetrisAI::CalcSteps()
{
    BlockState dest = CalcBestState();
    if (dest.index = -1){
        steps.clear();
        return;
    }
    CalcPath(dest, 1);
    return;
}

BlockState TetrisAI::GetNextStep(){
    if (steps.size() == currentStepIndex){
        CalcSteps();
        currentStepIndex = 0;
    }
    return steps[currentStepIndex++];
}
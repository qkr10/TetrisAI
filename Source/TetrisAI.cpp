#include "TetrisAI.hpp"

#include <algorithm>
#include <cmath>
#include <queue>
#include <tuple>
#include "Tetris.hpp"
#include "Output.hpp"

vp posSteps({
    {0, -1}, {-1, 0}, {1, 0}
});
vvvi visitBoard;

/***********************/
/*      함수 정의      */
/***********************/

bool TetrisAI::GetPathFromVisit(vvvi& visit, BlockState start, BlockState dest)
{
    auto [pos, rot, index, x, y] = start;

    for (int i = 0; i < posSteps.size(); i++){
        Point p = pos + posSteps[i];
        BlockState s = {p, rot, index};
        if (s.GetAround(board) == BOARD_STATE::EMPTY &&
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
        if (a.GetAroundSpinRev(board, res) == BOARD_STATE::EMPTY &&
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
    auto [pos, rot, index, x, y] = dest;
    Point startPos = {START_X, START_Y};

    visitBoard.clear();
    visitBoard.assign(BW + 2, vvi(BH + 2, vi(4, -1)));
    visitBoard[x][y][rot] = 0;

    using DSPair = std::pair<double, BlockState>;
    using PQ = std::priority_queue<DSPair, std::vector<DSPair>, std::greater<DSPair>>;
    PQ pq;
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
            if (s.GetAround(board) == BOARD_STATE::EMPTY &&
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
            if (a.GetAroundSpinRev(board, res) == BOARD_STATE::EMPTY &&
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
    int count = 0;

    BlockState state = {{1, 1}, 0, blocksShapeIndex[0]};
    auto& [pos, rot, index, x, y] = state;
    BlocksState states;
    for (y = BH; y > 0; y--){
        for (x = 1; x <= BW; x++){
            if (board[x][y] == BOARD_STATE::BRICK)
                continue;

            for (rot = 0; rot < 4; rot++){
                count++;
                BlockState b = {{x, y + 1}, rot, index};
                if (state.GetAround(board) == BOARD_STATE::EMPTY &&
                    b.GetAround(board) >= BOARD_STATE::BRICK){
                    if (CalcPath(state)){
                        states.push_back(state);
                    }
                }
            }
        }
    }
    if (states.size() == 0)
        return {{-1, -1}, -1, -1};

    Output::consoleOut << "loop count : " << count << std::endl;

    Point start(START_X, START_Y);
    sort(states.begin(), states.end(), [&](BlockState a, BlockState b)
        {
            return start.dist(a.pos) > start.dist(b.pos);
        });

    for (int i = 0; i < states.size(); i++)
    {
        Output::consoleOut << states[i] << std::endl;
    }

    return states[0];
}

void TetrisAI::CalcSteps()
{
    BlockState dest = CalcBestState();

    if (dest.index == -1){
        steps.clear();
        return;
    }
    CalcPath(dest, 1);
    return;
}

BlockState TetrisAI::GetNextStep(){
    Output::consoleOut << "index : " << blocksShapeIndex[0] << std::endl;
    if (steps.size() == currentStepIndex){
        CalcSteps();
        currentStepIndex = 0;
    }
    return steps[currentStepIndex++];
}
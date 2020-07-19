#include "TetrisAI.hpp"

extern Point Shape[][4][4];
extern int spinCenter[10][2];
//Main.cpp에 정의

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

double TetrisAI::dist(const Point a, const Point b)
{
    int x = a.x - b.x;
    int y = a.y - b.y;
    return sqrt(x * x + y * y);
}

int TetrisAI::GetAroundSpinRev(BlockState a, Point& ret)
{
    auto [pos, rot, index] = a;
    auto [x, y] = pos;
	for (int j = 0; j < 10; j++) {
		int xx = x - spinCenter[j][0];
		int yy = y - spinCenter[j][1];
		if (GetAround(x + xx, y + yy, index, rot) == EMPTY) {
			ret.x = xx;
			ret.y = yy;
			return EMPTY;
		}
	}
	return !EMPTY;
}

bool TetrisAI::GetPathFromArr(vvvi& arr, BlockState start, BlockState dest)
{
    auto [pos, rot, index] = start;
    auto [x, y] = pos;

    for (int i = 0; i < posSteps.size(); i++){
        Point p = pos + posSteps[i];
        if (GetAround(p.x, p.y, dest.BlockIndex, rot) == BOARD_STATE::EMPTY &&
            arr[p.x][p.y][rot] == arr[x][y][rot] - 1){
            steps.push_back({p, rot, index});
            if (GetPathFromArr(arr, {p, rot, index}, dest))
                return true;
            steps.pop_back();
        }
    }

    for (int r = 0; r < 4; r++){
        if (rot == r)
            continue;
        BlockState a = start;
        a.BlockRot = r;
        Point res;
        if (GetAroundSpinRev(a, res) == BOARD_STATE::EMPTY &&
            (visitBoard[res.x][res.y][r] == -1 || visitBoard[res.x][res.y][r] > visitBoard[x][y][rot])){
            visitBoard[res.x][res.y][r] = visitBoard[x][y][rot] + 1;
            a.BlockPos = res;
            pq.push(DSPair(dist(res, startPos), a));
        }
    }
}

bool TetrisAI::IsReachable(BlockState dest)
{
    visitBoard.clear();
    visitBoard.assign(BW, vvi(BH, vi(4, -1)));

    auto [pos, rot, index] = dest;
    auto [x, y] = pos;
    Point startPos = {START_X, START_Y};

    using DSPair = std::pair<double, BlockState>;
    using PQ = std::priority_queue<DSPair, std::vector<DSPair>, std::greater<DSPair>>;
    PQ pq;

    visitBoard[x][y][rot] = 0;
    pq.push(DSPair(dist(pos, startPos), dest));
    while (!pq.empty()){
        auto [dis, state] = pq.top();
        auto [pos, rot, index] = state;
        auto [x, y] = pos;
        pq.pop();

        if (pos == startPos){
            GetPathFromArr(visitBoard, {startPos, 0, index}, dest);
            return true;
        }

        for (int i = 0; i < posSteps.size(); i++){
            Point p = pos + posSteps[i];
            BlockState s = {p, rot, index};
            if (GetAround(p.x, p.y, index, rot) == BOARD_STATE::EMPTY &&
                (visitBoard[p.x][p.y][rot] == -1 || visitBoard[p.x][p.y][rot] > visitBoard[x][y][rot])){
                visitBoard[p.x][p.y][rot] = visitBoard[x][y][rot] + 1;
                pq.push(DSPair(dist(p, startPos), s));
            }
        }

        for (int r = 0; r < 4; r++){
            if (state.BlockRot == r)
                continue;
            BlockState a = state;
            a.BlockRot = r;
            Point res;
            if (GetAroundSpinRev(a, res) == BOARD_STATE::EMPTY &&
                (visitBoard[res.x][res.y][r] == -1 || visitBoard[res.x][res.y][r] > visitBoard[x][y][rot])){
                visitBoard[res.x][res.y][r] = visitBoard[x][y][rot] + 1;
                a.BlockPos = res;
                pq.push(DSPair(dist(res, startPos), a));
            }
        }
    }
    return false;
}

BlockState TetrisAI::CalcBestState()
{
    BlockState state = {{0, 0}, 0, blocksShapeIndex[0]};
    auto& [pos, rot, index] = state;
    auto& [x, y] = pos;
    BlocksState states;
    for (; y >= 0; y--){
        for (; x < 10; x++){
            if (board[x][y] == BOARD_STATE::BRICK)
                continue;
            for (; rot < 4; rot++){
                if (GetAround(x, y, index, rot) == BOARD_STATE::EMPTY &&
                    GetAround(x, y+1, index, rot) > BOARD_STATE::EMPTY){
                    if (IsReachable(state)){
                        states.push_back(state);
                    }
                }
            }
        }
    }
    if (states.size() == 0)
        return {{-1, -1}, -1, -1};

    sort(states.begin(), states.end(), [this](BlockState a, BlockState b)
        {
            return dist(a.BlockPos, Point(START_X, START_Y)) > dist(b.BlockPos, Point(START_X, START_Y));
        });

    return states[0];
}

void TetrisAI::CalcSteps()
{
    BlockState dest = CalcBestState();
    if (dest.BlockIndex = -1){
        steps.clear();
        return;
    }
    

    return;
}

BlockState TetrisAI::GetNextStep(){
    if (steps.size() == currentStepIndex){
        CalcSteps();
        currentStepIndex = 0;
    }
    return steps[currentStepIndex++];
}
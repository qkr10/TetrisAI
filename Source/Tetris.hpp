#pragma once

#include "TetrisVariables.hpp"
#include "TetrisAI.hpp"
#include "BlockState.hpp"
#include "Output.hpp"
#include "Input.hpp"
#include <vector>

using vi = std::vector<int>;
using vvi = std::vector<vi>;

class Tetris : public TetrisVariables
{
private:
    int box[7] = { 0, 1, 2, 3, 4, 5, 6 };
public:
    TetrisAI ai;
    Output out;
    Input in;
    vvi board;
    BlockState curState;
    
    int hbrick = -1;
    int nbrick;
    int infoScore;
    int infoBrickNum;
    int HoldTrig = 1;
    int DropTime = 1e+10;
    int moveDownTime = 20;
    int aiMoveTime = 2;

    void TetrisMain();
    void GameInit();
    void ProcessTimer();
    bool ProcessKey();
    void AINextStep();
    void AIInformState();
    bool MoveDown();
    void TestFull();
    void HoldBrick();
    void NewBrick();
    int GetNextBrick();
    void Shuffle();

    void Delay(int n) { Sleep(n); }
    void InitRandom() { srand((unsigned)time(NULL)); }
    int Random(int n) { return rand() % n; }
};
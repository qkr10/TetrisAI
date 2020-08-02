#pragma once

#include "Tetris.hpp"
#include "TetrisAI.hpp"
#include "BlockState.hpp"

class Tetris
{
private:
    int box[7] = { 0, 1, 2, 3, 4, 5, 6 };
public:
    int board[BW + 2][BH + 2];
    TetrisAI ai;
    BlockState curState;
    int nbrick;
    int infoScore;
    int infoBrickNum;
    int hbrick = -1;
    int HoldTrig = 1;
    int DropTime = 1e+10;
    int moveDownTime = 20;
    int aiMoveTime = 10;

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
};
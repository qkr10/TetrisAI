#pragma once

#include "Tetris.hpp"

void GameInit();
void ProcessTimer();
bool ProcessKey();
bool MoveDown();
void TestFull();
void HoldBrick();
void Shuffle();
int GetNextBrick();
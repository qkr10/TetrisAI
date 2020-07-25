#pragma once

#include "Tetris.hpp"
#include "Point.hpp"

void DrawScreen();
void DrawBoard();
BOOL ProcessKey();
void PrintBrick(BOOL Show);
// 벽돌의 이동/회전 가능성 조사(O : EMPTY, X : BRICK)
// x, y, b, r = 블럭상태(위치, 블럭종류, 회전한횟수)
// 회전이 불가능할때는 블럭이 이동할수 있으므로 GetAroundSpin써야함
int GetAround(int x, int y, int b, int r);
// 벽돌의 이동/회전 가능성 조사(O : EMPTY, X : BRICK)
// x, y, b, r = 처음 블럭상태(위치, 블럭종류, 회전한횟수)
// retx, rety = 움직인 후 블럭 위치
int GetAroundSpin(int x, int y, int b, int r, int* retx, int* rety);
BOOL MoveDown();
void TestFull();
void DrawNext();
void PrintInfo();
void HoldBrick();
void HoldScreen();
void Shuffle();
int GetNextBrick(int previousBrick);

void clrscr();
void gotoxy(int x, int y);
void gotoxy(Point p);
int wherex();
int wherey();
void setcursortype(CURSOR_TYPE c);
__inline void swap(int *a, int *b) {int temp = *a;*a = *b;*b = temp;}
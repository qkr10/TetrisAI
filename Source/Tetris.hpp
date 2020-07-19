#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include "Point.hpp"

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;
enum BOARD_STATE { EMPTY, BRICK, WALL };

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
int wherex();
int wherey();
void setcursortype(CURSOR_TYPE c);
__inline void swap(int *a, int *b) {int temp = *a;*a = *b;*b = temp;}

#define delay(n) Sleep(n)							// n/1000초만큼 시간 지연
#define randomize() srand((unsigned)time(NULL))		// 난수 발생기 초기화
#define random(n) (rand() % (n))					//0~n까지의 난수 발생

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ESC 27
#define BX 5
#define BY 1
#define BW 10
#define BH 20
#define PGUP 73
#define PGDN 81
#define HOLD 104
#define DO_SUFFLE -1
#define START_X BW / 2
#define START_Y 3
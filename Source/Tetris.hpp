#pragma once

enum CURSOR_TYPE { NOCURSOR, SOLIDCURSOR, NORMALCURSOR };
enum BOARD_STATE { EMPTY, BRICK, WALL };

#define delay(n) Sleep(n)							// n/1000초만큼 시간 지연
#define randomize() srand((unsigned)time(NULL))		// 난수 발생기 초기화
#define random(n) (rand() % (n))					//0~n까지의 난수 발생

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ESC 27
#define HOLD_X 54
#define HOLD_Y 15
#define NEXT_X 40
#define NEXT_Y 15
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
#include "Output.hpp"
#include "Shape.hpp"
#include <windows.h>
#include <conio.h>
#include <cstdio>

char* Output::arTile[4][3] = {
	{"[]", "■",  "□"},
	{"  ", "■",  "□"},
	{"  ", "##", "II"},
	{"  ", "●",  "▣"}
};

int Output::ttype = 0;

void Output::SetCursorType(CURSOR_TYPE c)
{
	CONSOLE_CURSOR_INFO CurInfo;

	switch (c) {
	case CURSOR_TYPE::NOCURSOR:
		CurInfo.dwSize = 1;
		CurInfo.bVisible = FALSE;
		break;
	case CURSOR_TYPE::SOLIDCURSOR:
		CurInfo.dwSize = 100;
		CurInfo.bVisible = TRUE;
		break;
	case CURSOR_TYPE::NORMALCURSOR:
		CurInfo.dwSize = 20;
		CurInfo.bVisible = TRUE;
		break;
	}
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

void Output::gotoxy(Point p)
{
	COORD Cur = {(short)p.x, (short)p.y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

void Output::PaintBrick(Point p)
{
    gotoxy(p);
    puts(arTile[ttype][1]);
}

void Output::PaintEmpty(Point p)
{
    gotoxy(p);
    puts(arTile[ttype][0]);
}

void Output::DrawHold(int hBrick)
{
	for (int x = 50; x <= 60; x += 2) {
		for (int y = 12; y <= 18; y++) {
			gotoxy(Point(x, y));
			puts(arTile[ttype][(x == 50 || x == 60 || y == 12 || y == 18) ? 2 : 0]);
		}
	}

	if (hBrick != -1) {
		for (int i = 0; i < 4; i++) {
			gotoxy(Shape::GetHoldPos(i, hBrick));
			puts(arTile[ttype][1]);
		}
	}
}

void Output::DrawNext(int nBrick) 
{
	for (int x = 36; x <= 46; x += 2) {
		for (int y = 12; y <= 18; y++) {
			gotoxy(Point(x, y));
			puts(arTile[ttype][(x == 36 || x == 46 || y == 12 || y == 18) ? 2 : 0]);

		}
	}

	for (int i = 0; i < 4; i++) {
		gotoxy(Shape::GetNextPos(i, nBrick));
		puts(arTile[ttype][1]);
	}
}

void Output::DrawBlock(bool show)
{
    for (int i = 0; i < 4; i++){
        gotoxy(Shape::GetBoardPos(i));
        puts(arTile[ttype][show ? 1 : 0]);
    }
}

void Output::DrawBoard(int (*board)[BH + 2])
{
	for (int x = 1; x < BW + 1; x++) {
		for (int y = 1; y < BH + 1; y++) {
			gotoxy(Point(BX + x * 2, BY + y));
			puts(arTile[ttype][board[x][y]]);
		}
	}
}

void Output::DrawScore(int infoScore, int infoBrickNum)          //점수와 벽돌의갯수를 나타내는 함수
{
	gotoxy(Point(50,  9)); printf("점수 : %d     ", infoScore);
	gotoxy(Point(50, 10)); printf("벽돌 : %d 개  ", infoBrickNum);
}

void Output::DrawInfo()
{
    gotoxy(Point(35,  2)); puts("뉴 테트리스!");
	gotoxy(Point(35,  4)); puts("이동 :← →, 회전 :↑, 내림 :↓");
	gotoxy(Point(35,  5)); puts("전부내림 : space bar, 종료 : ESC");
	gotoxy(Point(35,  6)); puts("일시정지 : P, 모양바꿈 : Page up,down");
	gotoxy(Point(35,  7)); puts("홀드 : h ");
	gotoxy(Point(40, 19)); printf("Next");
	gotoxy(Point(54, 19)); printf("Hold");
}

void Output::DrawStart()
{
    SetCursorType(CURSOR_TYPE::NOCURSOR);
	gotoxy(Point(27,  9)); puts("┏━━━━━━━━━━━━━━━━━━━━━━┓");
	gotoxy(Point(27, 10)); puts("┃      뉴테트리스      ┃");
	gotoxy(Point(27, 11)); puts("┃                      ┃");
	gotoxy(Point(27, 12)); puts("┃ 게임시작-> space bar ┃");
	gotoxy(Point(27, 13)); puts("┗━━━━━━━━━━━━━━━━━━━━━━┛");
}

void Output::DrawEnd()
{
    gotoxy(Point(30, 12)); puts("G A M E  O V E R");
    gotoxy(Point(25, 14)); puts("다시 시작하려면 y를 누르세요");
}

void Output::Clear()
{
    system("cls");
}
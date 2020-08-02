#include "Output.hpp"
#include "BlockState.hpp"
#include <windows.h>
#include <conio.h>
#include <cstdio>
#include <fstream>
#include <string>

char* Output::arTile[4][3] = {
	{"  ", "■",  "□"},
	{"ㅁ", "■",  "□"},
	{"  ", "##", "II"},
	{"  ", "▣",  "●▣"}
};

int Output::ttype = 0;

std::ofstream Output::consoleOut("console.txt");

void Output::SetCursorType(int c)
{
	CONSOLE_CURSOR_INFO CurInfo;

	switch (c) {
	case 0: //NOCURSOR
		CurInfo.dwSize = 1;
		CurInfo.bVisible = FALSE;
		break;
	case 1: //SOLIDCURSOR
		CurInfo.dwSize = 100;
		CurInfo.bVisible = TRUE;
		break;
	case 2: //NORMALCURSOR
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

void Output::ChangeTileSet(int i)
{
	ttype = (ttype + i + 4) % 4;
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
			gotoxy(BlockState::GetHoldPos(i, hBrick));
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
		gotoxy(BlockState::GetNextPos(i, nBrick));
		puts(arTile[ttype][1]);
	}
}

void Output::DrawBlock(BlockState curState, bool show)
{
    for (int i = 0; i < 4; i++){
        gotoxy(curState.GetBoardPos(i));
        puts(arTile[ttype][show ? 1 : 0]);
    }
}

void Output::DrawBoard(int (*board)[BH + 2])
{
	for (int x = 0; x < BW + 2; x++) {
		for (int y = 0; y < BH + 2; y++) {
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

void Output::DrawScreen(int (*board)[BH + 2], BlockState& curState, int nbrick, int hbrick, int infoScore, int infoBrickNum)
{
	Output::DrawBoard(board);
	Output::DrawInfo();
	Output::DrawNext(nbrick);
	Output::DrawHold(hbrick);
	Output::DrawScore(infoScore, infoBrickNum);
	Output::DrawBlock(curState, true);
}

void Output::DrawStart()
{
    SetCursorType(0);
	gotoxy(Point(27,  9)); puts("┏━━━━━━━━━━━━━━━━━━━━━━┓");
	gotoxy(Point(27, 10)); puts("┃      뉴테트리스      ┃");
	gotoxy(Point(27, 11)); puts("┃                      ┃");
	gotoxy(Point(27, 12)); puts("┃ 게임시작-> space bar ┃");
	gotoxy(Point(27, 13)); puts("┗━━━━━━━━━━━━━━━━━━━━━━┛");
}

void Output::DrawPause()
{
	gotoxy(Point(15, 10));
	puts("Tetris 잠시 중지. 다시 시작하려면 아무 키나 누르세요!");
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

std::ostream &operator<<(std::ostream& os, BlockState s)
{
	os << s.pos << ' ' << s.rot << ' ' << s.index;
	return os;
}

std::ostream &operator<<(std::ostream& os, Point p)
{
	os << p.x << ' ' << p.y;
	return os;
}
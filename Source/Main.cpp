#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include "Main.hpp"
#include "BlockState.hpp"
#include "Shape.hpp"
#include "Input.hpp"
#include "Output.hpp"

char *arTile[][3] = {             //테트리스의 모양을 바꿔준다
	 {"[]","■","□"},
	 {"  ","■","□"},
	 {"  ","##","II"},
	 {"  ","●","▣"},
};

Point spinCenter[10] = {
	{0, 0},{1, 0},{-1, 0},
	{0, 1},{1, 1},{-1, 1},
	{0, 2},{1, 2},{-1, 2},
	{0, -1}
};

int box[] = { 0, 1, 2, 3, 4, 5, 6 };

int board[BW + 2][BH + 2];
BlockState curState;
int ttype = 0;
int nbrick;
int infoScore;
int infoBrickNum;
int start;
int hbrick = -1;
int HoldTrig = 1;
int DropTime = 1e+10;

void main()
{
	int nFrame, nStay;
	int x, y;
	start = 1;

	Output::DrawStart();
	Input::WaitKeyPress(' ');

	randomize();   //난수 발생기 초기화
	for (; 3;) {
		Output::Clear();     //화면을 지움
		for (x = 0; x < BW + 2; x++) {       //BW는 테트리스의 가로길이
			for (y = 0; y < BH + 2; y++) {      //BH는 테트리스의 세로길이
				board[x][y] = (y == 0 || y == BH + 1 || x == 0 || x == BW + 1) ? WALL : EMPTY;    //board 배열에 WALL이나 EMPTY를 넣음
			}
		}       //BW 와 BH는 벽돌이 실제 움직이는 공간이므로 +2씩 한다
		DrawScreen();
		nFrame = 20;
		infoScore = 0;
		infoBrickNum = 0;

		Shuffle();
		nbrick = GetNextBrick(nbrick);
		for (; 1;) {
			infoBrickNum++;
			curState = {{START_X, START_Y}, 0, nbrick};
			nbrick = GetNextBrick(nbrick);
			Output::DrawNext(nbrick);
			Output::DrawBlock(TRUE);

			if (curState.GetAround() != EMPTY) break;
			nStay = nFrame;
			for (; 2;) {
				if (--nStay == 0) {
					nStay = nFrame;
					if (MoveDown()) break;
				}
				if (ProcessKey()) break;
				delay(1000 / 20);
			}
			if (infoBrickNum % 10 == 0 && nFrame > 5) {
				nFrame--;
			}
		}
		Output::Clear();
		Output::DrawEnd();
		Input::WaitKeyPress('y');
	}
}

void DrawScreen()
{
	Output::DrawBoard(board);
	Output::DrawInfo();
	Output::DrawNext(nbrick);
	Output::DrawHold(hbrick);
	Output::DrawScore(infoScore, infoBrickNum);
}

bool ProcessKey()          // 키입력을 처리하는데 main함수의 부담을 덜어주기 위해 별도의 함수로 분리
{                          // 이동중인 벽돌이 바닥에 닿으면 TRUE를 리턴
	int ch;
	bool ret = false;
	BlockState newState(curState);

	auto MoveFn = [&](Point move){
		newState = newState + (BlockState)move;
		if (newState.GetAround() == EMPTY) {
			Output::DrawBlock(FALSE);
			curState = newState;
			Output::DrawBlock(TRUE);
			DropTime = clock();
		}
	};
	auto RotFn = [&](BlockState rot){
		newState = newState + (BlockState)rot;
		if (newState.GetAroundSpin(newState.pos) == EMPTY) {
			Output::DrawBlock(FALSE);
			curState = newState;
			Output::DrawBlock(TRUE);
			DropTime = clock();
		}
	};

	switch (Input::GetInput())
	{
	case InputKind::UP:
		RotFn(BlockState({0, 0}, 1, 0));
		break;
	case InputKind::DOWN:
		ret = MoveDown();
		break;
	case InputKind::LEFT:
		MoveFn(Point(-1, 0));
		break;
	case InputKind::RIGHT:
		MoveFn(Point(1, 0));
		break;
	case InputKind::HOLD:
		HoldBrick();
		Output::DrawHold(hbrick);
		break;
	case InputKind::LEFTROT:
		RotFn(BlockState({0, 0}, 1, 0));
		break;
	case InputKind::RIGHTROT:
		RotFn(BlockState({0, 0}, -1, 0));
		break;
	case InputKind::DROP:
		while (MoveDown() == FALSE) ;
		return true;
		break;
	case InputKind::ESC:
		exit(0);
		break;
	case InputKind::PAUSE:
		Output::DrawHold(hbrick);
		Output::Clear();
		gotoxy(15, 10);
		puts("Tetris 잠시 중지. 다시 시작하려면 아무 키나 누르세요!");
		getch();
		Output::Clear();
		DrawScreen();
		Output::DrawBlock(TRUE);
		break;
	case InputKind::OPT0:
		if (ttype == 0)
			ttype = sizeof(arTile) / sizeof(arTile[0]);
		ttype--;
		DrawScreen();
		Output::DrawBlock(TRUE);
		break;
	case InputKind::OPT1:
		ttype++;
		if (ttype == sizeof(arTile) / sizeof(arTile[0]))
			ttype = 0;
		DrawScreen();
		Output::DrawBlock(TRUE);
		break;
	}

	return ret;
}

bool MoveDown()   //벽돌을 한칸 아래로 이동시킨다.
{
	BlockState newState = curState + BlockState({0, 1}, 0, 0);
	if (newState.GetAround() != 0) {
		if (DropTime + CLOCKS_PER_SEC / 2 >= clock())
			return false;
		HoldTrig = 1;
		TestFull();
		return true;//바닥에 닿았다면 TestFull() 한 후 TRUE를 리턴한다.
	}
	Output::DrawBlock(false);
	curState = newState;
	DropTime = clock();
	Output::DrawBlock(true);
	return false;
}

void TestFull()              //수평으로 다 채워진 줄을 찾아 삭제한다
{
	int i, x, y, ty;
	int count = 0;
	static int arScoreInc[] = { 0,1,3,8,20 };

	for (i = 0; i < 4; i++) {
		board[Shape::GetPos(i).x][Shape::GetPos(i).y] = BRICK;
	}

	for (y = 1; y < BH + 1; y++) {
		for (x = 1; x < BW + 1; x++) {
			if (board[x][y] != BRICK) break;
		}
		if (x == BW + 1) {
			count++;
			for (ty = y; ty > 1; ty--) {
				for (x = 1; x < BW + 1; x++) {
					board[x][ty] = board[x][ty - 1];
				}
			}
			Output::DrawBoard(board);
			delay(200);
		}
	}
	infoScore += arScoreInc[count];
	Output::DrawScore(infoScore, infoBrickNum);
}

void HoldBrick() { //블럭을 홀드한다
	int temp = 0, i = 0;
	HoldTrig = 0;
	if (hbrick == 8)
	{
		hbrick = curState.index;
		curState.index = nbrick;
		nbrick = GetNextBrick(nbrick);
	}
	else
	{
		temp = curState.index;
		curState.index = hbrick;
		hbrick = temp;
	}
	curState.pos = Point(BW / 2, 3);
	Output::DrawBlock(true);
}

void Shuffle() {
	//Fisher–Yates shuffle 알고리즘
	int* start = box;
	for (int i = 0; i < 7; i++) {
		swap(start, start + random(7 - i));
		start++;
	}
}

int GetNextBrick(int previousBrick) {
	if (previousBrick == box[6]) {
		Shuffle();
		return box[0];
	}
	int i = 0;
	while (box[i++] != previousBrick);
	return box[i];
}

// 커서를 x,y좌표로 이동시킨다.
void gotoxy(int x, int y)
{
	COORD Cur = {(short)x, (short)y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}
void gotoxy(Point p)
{
	COORD Cur = {(short)p.x, (short)p.y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}
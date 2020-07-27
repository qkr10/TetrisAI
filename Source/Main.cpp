#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include <algorithm>
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

int box[] = { 0, 1, 2, 3, 4, 5, 6 };

int board[BW + 2][BH + 2];
BlockState curState;
int nbrick;
int infoScore;
int infoBrickNum;
int hbrick = -1;
int HoldTrig = 1;
int DropTime = 1e+10;
int moveDownTime = 20;

void main()
{
	Output::DrawStart();
	Input::WaitKeyPress(' ');

	randomize();   //난수 발생기 초기화
	while (true) {
		GameInit();
		while (true) {
			infoBrickNum++;
			curState = {{START_X, START_Y}, 0, nbrick};
			nbrick = GetNextBrick();
			Output::DrawNext(nbrick);
			Output::DrawScreen(board, nbrick, hbrick, infoScore, infoBrickNum);
			Output::DrawBlock(TRUE);
			if (curState.GetAround() != EMPTY) break;
			ProcessTimer();
		}
		Output::Clear();
		Output::DrawEnd();
		Input::WaitKeyPress('y');
	}
}

void GameInit()
{
	Output::Clear();     //화면을 지움
	for (int x = 0; x < BW + 2; x++) {       //BW는 테트리스의 가로길이
		for (int y = 0; y < BH + 2; y++) {      //BH는 테트리스의 세로길이
			board[x][y] = (y == 0 || y == BH + 1 || x == 0 || x == BW + 1) ? WALL : EMPTY;    //board 배열에 WALL이나 EMPTY를 넣음
		}
	}       //BW 와 BH는 벽돌이 실제 움직이는 공간이므로 +2씩 한다
	infoScore = 0;
	infoBrickNum = 0;

	Shuffle();
}

void ProcessTimer()
{
	int moveDown = moveDownTime;
	while (true) {
		if (--moveDown == 0) {
			moveDown = moveDownTime;
			if (MoveDown()) break;
		}
		if (ProcessKey()) break;
		delay(1000 / 20);
	}
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
		Output::DrawPause();
		Input::WaitKeyPress();
		Output::Clear();
		Output::DrawScreen(board, nbrick, hbrick, infoScore, infoBrickNum);
		break;
	case InputKind::OPT0:
		Output::ChangeTileSet(-1);
		Output::DrawScreen(board, nbrick, hbrick, infoScore, infoBrickNum);
		break;
	case InputKind::OPT1:
		Output::ChangeTileSet(1);
		Output::DrawScreen(board, nbrick, hbrick, infoScore, infoBrickNum);
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
		nbrick = GetNextBrick();
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
	for (int i = 0; i < 7; i++)
		std::swap(box[i], box[i + random(7 - i)]);
}

int GetNextBrick() {
	static int i = 0;
	if (i == 7) {
		i = 0;
		Shuffle();
	}
	return box[i++];
}
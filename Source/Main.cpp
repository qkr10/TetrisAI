#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <algorithm>
#include "Output.hpp"
#include "BlockState.hpp"
#include "Input.hpp"
#include "TetrisAI.hpp"
#include "Main.hpp"

int main()
{
	Tetris game;
	Output::DrawStart();
	Input::WaitKeyPress(' ');

	randomize();   //난수 발생기 초기화
	while (true) {
		game.GameInit();
		while (true) {
			game.NewBrick();
			Output::DrawScreen(game.board, game.curState, game.nbrick, game.hbrick, game.infoScore, game.infoBrickNum);
			if (game.curState.GetAround(game.board) != EMPTY) break;
			game.ProcessTimer();
		}
		Output::Clear();
		Output::DrawEnd();
		Input::WaitKeyPress('y');
	}
}

void Tetris::GameInit()
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
	nbrick = GetNextBrick();
}

void Tetris::ProcessTimer()
{
	int moveDown = moveDownTime;
	int aiMove = aiMoveTime;
	while (true) {
		// if (--moveDown == 0) {
		// 	moveDown = moveDownTime;
		// 	if (MoveDown()) break;
		// }
		if (--aiMove == 0) {
			aiMove = aiMoveTime;
			AINextStep();
		}
		if (ProcessKey()) break;
		delay(1000 / 20);
	}
}

bool Tetris::ProcessKey()          // 키입력을 처리하는데 main함수의 부담을 덜어주기 위해 별도의 함수로 분리
{                          // 이동중인 벽돌이 바닥에 닿으면 TRUE를 리턴
	int ch;
	bool ret = false;
	BlockState newState(curState);

	auto MoveFn = [&](Point move){
		newState = newState + (BlockState)move;
		if (newState.GetAround(board) == EMPTY) {
			Output::DrawBlock(curState, FALSE);
			curState = newState;
			Output::DrawBlock(curState, TRUE);
			DropTime = clock();
		}
	};
	auto RotFn = [&](BlockState rot){
		newState = newState + (BlockState)rot;
		if (newState.GetAroundSpin(board, newState.pos) == EMPTY) {
			Output::DrawBlock(curState, FALSE);
			curState = newState;
			Output::DrawBlock(curState, TRUE);
			DropTime = clock();
		}
	};

	switch (Input::GetInput())
	{
	case InputEnum::KEYUP:
		RotFn(BlockState({0, 0}, 1, 0));
		break;
	case InputEnum::KEYDOWN:
		ret = MoveDown();
		break;
	case InputEnum::KEYLEFT:
		MoveFn(Point(-1, 0));
		break;
	case InputEnum::KEYRIGHT:
		MoveFn(Point(1, 0));
		break;
	case InputEnum::KEYHOLD:
		HoldBrick();
		Output::DrawHold(hbrick);
		break;
	case InputEnum::KEYLEFTROT:
		RotFn(BlockState({0, 0}, 1, 0));
		break;
	case InputEnum::KEYRIGHTROT:
		RotFn(BlockState({0, 0}, -1, 0));
		break;
	case InputEnum::KEYDROP:
		while (MoveDown() == FALSE) ;
		return true;
		break;
	case InputEnum::KEYESC:
		exit(0);
		break;
	case InputEnum::KEYPAUSE:
		Output::DrawHold(hbrick);
		Output::Clear();
		Output::DrawPause();
		Input::WaitKeyPress();
		Output::Clear();
		Output::DrawScreen(board, curState, nbrick, hbrick, infoScore, infoBrickNum);
		break;
	case InputEnum::KEYOPT0:
		Output::ChangeTileSet(-1);
		Output::DrawScreen(board, curState, nbrick, hbrick, infoScore, infoBrickNum);
		break;
	case InputEnum::KEYOPT1:
		Output::ChangeTileSet(1);
		Output::DrawScreen(board, curState, nbrick, hbrick, infoScore, infoBrickNum);
		break;
	}

	return ret;
}

void Tetris::AINextStep()
{
	BlockState nextState = ai.GetNextStep();
	BlockState diffState = nextState - curState;
	if (diffState.x == 1)
		Input::next = InputEnum::KEYRIGHT;
	else if (diffState.x == -1)
		Input::next = InputEnum::KEYLEFT;
	else if (diffState.y == 1)
		Input::next = InputEnum::KEYDOWN;
	else if (diffState.rot == -1)
		Input::next = InputEnum::KEYLEFTROT;
	else if (diffState.rot == 1)
		Input::next = InputEnum::KEYRIGHTROT;
	else if (diffState.index != 0)
		Input::next = InputEnum::KEYHOLD;
}

void Tetris::AIInformState()
{
	for (int x = 0; x < BW+2; x++)
	for (int y = 0; y < BW+2; y++)
		ai.board[x][y] = board[x][y];

	if (ai.blocksShapeIndex.size() == 0)
		ai.blocksShapeIndex.push_back(0);
	ai.blocksShapeIndex[0] = curState.index;
}

bool Tetris::MoveDown()   //벽돌을 한칸 아래로 이동시킨다.
{
	BlockState newState = curState + BlockState({0, 1}, 0, 0);
	if (newState.GetAround(board) != 0) {
		if (DropTime + CLOCKS_PER_SEC / 2 >= clock())
			return false;
		HoldTrig = 1;
		TestFull();
		return true;//바닥에 닿았다면 TestFull() 한 후 TRUE를 리턴한다.
	}
	Output::DrawBlock(curState, false);
	curState = newState;
	DropTime = clock();
	Output::DrawBlock(curState, true);
	return false;
}

void Tetris::TestFull()              //수평으로 다 채워진 줄을 찾아 삭제한다
{
	int i, x, y, ty;
	int count = 0;
	static int arScoreInc[] = { 0,1,3,8,20 };

	for (i = 0; i < 4; i++) {
		board[curState.GetPos(i).x][curState.GetPos(i).y] = BRICK;
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

void Tetris::HoldBrick()
{ //블럭을 홀드한다
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
	Output::DrawBlock(curState, true);
}

void Tetris::NewBrick()
{
	infoBrickNum++;
	curState = {{START_X, START_Y}, 0, nbrick};
	nbrick = GetNextBrick();
	AIInformState();
}

void Tetris::Shuffle()
{
	//Fisher–Yates shuffle 알고리즘
	for (int i = 0; i < 7; i++)
		std::swap(box[i], box[i + random(7 - i)]);
}

int Tetris::GetNextBrick()
{
	static int i = 0;
	if (i == 7) {
		i = 0;
		Shuffle();
	}
	return box[i++];
}
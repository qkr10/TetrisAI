#include <algorithm>
#include "Output.hpp"
#include "BlockState.hpp"
#include "Input.hpp"
#include "TetrisAI.hpp"
#include "Tetris.hpp"

void Tetris::TetrisMain()
{
	board.assign(boardSize.x + 2, vi(boardSize.y + 2, 0));
	ai.board.assign(boardSize.x + 2, vi(boardSize.y + 2, 0));
	
	out.DrawStart();
	in.WaitKeyPress(' ');

	InitRandom();   //난수 발생기 초기화
	while (true) {
		GameInit();
		while (true) {
			NewBrick();
			out.DrawScreen(board, curState, nbrick, hbrick, infoScore, infoBrickNum);
			if (curState.GetAround(board) != EMPTY) break;
			ProcessTimer();
		}
		out.Clear();
		out.DrawEnd();
		in.WaitKeyPress('y');
	}
}

void Tetris::GameInit()
{
	out.Clear();     //화면을 지움
	for (int x = 0; x < boardSize.x + 2; x++) {
		for (int y = 0; y < boardSize.y + 2; y++) {
			board[x][y] = (y == 0 || y == boardSize.y + 1 || x == 0 || x == boardSize.x + 1) ? WALL : EMPTY;
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
		Delay(50);
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
			out.DrawBlock(curState, FALSE);
			curState = newState;
			out.DrawBlock(curState, TRUE);
			//DropTime = clock();
		}
	};
	auto RotFn = [&](BlockState rot){
		newState = newState + (BlockState)rot;
		if (newState.GetAroundSpin(board, newState) == EMPTY) {
			out.DrawBlock(curState, FALSE);
			curState = newState;
			out.DrawBlock(curState, TRUE);
			//DropTime = clock();
		}
	};

	switch (in.GetAIInput())
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
		out.DrawHold(hbrick);
		break;
	case InputEnum::KEYLEFTROT:
		RotFn(BlockState({0, 0}, 1, 0));
		break;
	case InputEnum::KEYRIGHTROT:
		RotFn(BlockState({0, 0}, -1, 0));
		break;
	case InputEnum::KEYDROP:
		while (MoveDown() == FALSE) ;
		ret = true;
		break;
	case InputEnum::KEYESC:
		exit(0);
		break;
	case InputEnum::KEYPAUSE:
		out.DrawHold(hbrick);
		out.Clear();
		out.DrawPause();
		in.WaitKeyPress();
		out.Clear();
		out.DrawScreen(board, curState, nbrick, hbrick, infoScore, infoBrickNum);
		break;
	case InputEnum::KEYOPT0:
		out.ChangeTileSet(-1);
		out.DrawScreen(board, curState, nbrick, hbrick, infoScore, infoBrickNum);
		break;
	case InputEnum::KEYOPT1:
		out.ChangeTileSet(1);
		out.DrawScreen(board, curState, nbrick, hbrick, infoScore, infoBrickNum);
		break;
	}

	return ret;
}

void Tetris::AINextStep()
{
	if (in.nextAIInput != InputEnum::NOINPUT)
		return;

	BlockState nextState = ai.GetNextStep();

	Output::consoleOut << "ai next step" << std::endl;
	Output::consoleOut << "\tnext state : " << nextState << std::endl;
	Output::consoleOut << "\tcurrent state : " << curState << std::endl;

	if (nextState.index == -1)
		in.nextAIInput = InputEnum::KEYHOLD;
	else if (nextState.index == -2)
		in.nextAIInput = InputEnum::KEYDROP;

	if (in.nextAIInput != InputEnum::NOINPUT)
		return;

	BlockState diffState = nextState - curState;

	if (diffState.x == 1)
		in.nextAIInput = InputEnum::KEYRIGHT;
	else if (diffState.x == -1)
		in.nextAIInput = InputEnum::KEYLEFT;
	else if (diffState.y == 1)
		in.nextAIInput = InputEnum::KEYDOWN;
	else if (diffState.rot == 1)
		in.nextAIInput = InputEnum::KEYLEFTROT;
	else if (diffState.rot == 3)
		in.nextAIInput = InputEnum::KEYRIGHTROT;
}

void Tetris::AIInformState()
{
	for (int y = 0; y < boardSize.y + 2; y++)
		for (int x = 0; x < boardSize.x + 2; x++)
			ai.board[x][y] = board[x][y];

	if (ai.blocksShapeIndex.size() == 0)
		ai.blocksShapeIndex.push_back(0);
	ai.blocksShapeIndex[0] = curState.index;
}

bool Tetris::MoveDown()   //벽돌을 한칸 아래로 이동시킨다.
{
	BlockState newState = curState + BlockState({0, 1}, 0, 0);
	if (newState.GetAround(board) > BOARD_STATE::EMPTY) {
		// if (DropTime + CLOCKS_PER_SEC / 2 >= clock())
		// 	return false;
		HoldTrig = 1;
		TestFull();
		if (ai.steps.size() != ai.currentStepIndex) {
			Output::consoleOut << "failed to expect" << std::endl;
			for (auto var : ai.steps) {
				Output::consoleOut << var << std::endl;
			}
			ai.steps.clear();
			ai.currentStepIndex = 0;
		}
		return true;//바닥에 닿았다면 TestFull() 한 후 TRUE를 리턴한다.
	}
	out.DrawBlock(curState, false);
	curState = newState;
	//DropTime = clock();
	out.DrawBlock(curState, true);
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

	for (y = 1; y < boardSize.y + 1; y++) {
		for (x = 1; x < boardSize.x + 1; x++) {
			if (board[x][y] != BRICK) break;
		}
		if (x == boardSize.x + 1) {
			count++;
			for (ty = y; ty > 1; ty--) {
				for (x = 1; x < boardSize.x + 1; x++) {
					board[x][ty] = board[x][ty - 1];
				}
			}
			out.DrawBoard(board);
			Delay(20);
		}
	}
	infoScore += arScoreInc[count];
	out.DrawScore(infoScore, infoBrickNum);
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
	curState = Point(boardSize.x / 2, 3);
	out.DrawBlock(curState, true);
}

void Tetris::NewBrick()
{
	infoBrickNum++;
	curState = BlockState(startCor, 0, nbrick);
	nbrick = GetNextBrick();
	AIInformState();
}

void Tetris::Shuffle()
{
	//Fisher–Yates shuffle 알고리즘
	for (int i = 0; i < 7; i++)
		std::swap(box[i], box[i + Random(7 - i)]);
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
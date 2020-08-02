#pragma once

#include "Point.hpp"
#include "BlockState.hpp"
#include "Tetris.hpp"
#include <string>
#include <fstream>

class Output
{
public:
    static char* arTile[4][3];
    static int ttype;
    static std::ofstream consoleOut;
    
    static void SetCursorType(int c);
    static void gotoxy(Point p);
    static void PaintBrick(Point p);
    static void PaintEmpty(Point p);
    static void ChangeTileSet(int i);

    //hBrick == -1 이면 빈공간 출력
    static void DrawHold(int hBrick);
    static void DrawNext(int nBrick);
    static void DrawBlock(BlockState curState, bool show);
    static void DrawBoard(int (*board)[BH + 2]);
    static void DrawScore(int infoScore, int infoBrickNum);
    static void DrawInfo();
    static void DrawScreen(int (*board)[BH + 2], BlockState& curState, int nbrick, int hbrick, int infoScore, int infoBrickNum);

    static void DrawStart();
    static void DrawPause();
    static void DrawEnd();
    static void Clear();
    static void ConsoleOutInit();
};

std::ostream &operator<<(std::ostream& os, BlockState s);
std::ostream &operator<<(std::ostream& os, Point s);
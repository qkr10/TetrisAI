#pragma once

#include "Point.hpp"
#include "BlockState.hpp"
#include "Tetris.hpp"

enum class CURSOR_TYPE { NOCURSOR, SOLIDCURSOR, NORMALCURSOR };

class Output
{
public:
    static char* arTile[4][3];
    static int ttype;
    
    static void SetCursorType(CURSOR_TYPE c);
    static void gotoxy(Point p);
    static void PaintBrick(Point p);
    static void PaintEmpty(Point p);
    static void ChangeTileSet(int i);

    //hBrick == -1 이면 빈공간 출력
    static void DrawHold(int hBrick);
    static void DrawNext(int nBrick);
    static void DrawBlock(bool show);
    static void DrawBoard(int (*board)[BH + 2]);
    static void DrawScore(int infoScore, int infoBrickNum);
    static void DrawInfo();
    static void DrawScreen(int (*board)[BH + 2], int nbrick, int hbrick, int infoScore, int infoBrickNum);

    static void DrawStart();
    static void DrawPause();
    static void DrawEnd();
    static void Clear();
};
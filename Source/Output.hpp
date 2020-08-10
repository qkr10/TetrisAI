#pragma once

#include "Point.hpp"
#include "BlockState.hpp"
#include "TetrisVariables.hpp"
#include <string>
#include <fstream>
#include <vector>

using vi = std::vector<int>;
using vvi = std::vector<vi>;

class Output
{
public:
    static std::ofstream consoleOut;
    
    char* arTile[4][3] = {
        {"  ", "■",  "□"},
        {"ㅁ", "■",  "□"},
        {"  ", "##", "II"},
        {"  ", "▣",  "●▣"}
    };
    int ttype = 0;
    Point boardLeftTop = Point(5, 1);
    Point holdLeftTop = Point(54, 15);
    Point nextLeftTop = Point(40, 15);

    void SetCursorType(int c);
    void gotoxy(Point p);
    void PaintBrick(Point p);
    void PaintEmpty(Point p);
    void ChangeTileSet(int i);

    //hBrick == -1 이면 빈공간 출력
    void DrawHold(int hBrick);
    void DrawNext(int nBrick);
    void DrawBlock(BlockState curState, bool show);
    void DrawBoard(vvi &board);
    void DrawScore(int infoScore, int infoBrickNum);
    void DrawInfo();
    void DrawScreen(vvi &board, BlockState& curState, int nbrick, int hbrick, int infoScore, int infoBrickNum);

    void DrawStart();
    void DrawPause();
    void DrawEnd();
    void Clear();
};

std::ostream &operator<<(std::ostream& os, BlockState s);
std::ostream &operator<<(std::ostream& os, Point s);
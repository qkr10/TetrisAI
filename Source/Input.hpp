#pragma once

enum class InputKind
{
    NONE,
    UP, 
    DOWN, 
    LEFT, 
    RIGHT, 
    HOLD, 
    LEFTROT, 
    RIGHTROT, 
    DROP, 
    ESC, 
    PAUSE, 
    OPT0, 
    OPT1, 
    OPT2
};

class Input
{
public:
    static InputKind GetInput();
    static void WaitKeyPress(char ch);
};
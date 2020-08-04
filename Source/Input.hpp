#pragma once

enum class InputEnum {
    NOINPUT,
    KEYUP,
    KEYDOWN,
    KEYLEFT,
    KEYRIGHT,
    KEYHOLD,
    KEYLEFTROT,
    KEYRIGHTROT,
    KEYDROP,
    KEYESC,
    KEYPAUSE,
    KEYOPT0,
    KEYOPT1,
    KEYOPT2
};

class Input
{
public:
    static InputEnum nextAIInput;
    static InputEnum GetAIInput();
    static InputEnum GetInput();
    static void WaitKeyPress(char ch = 0);
};
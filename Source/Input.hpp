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
    InputEnum nextAIInput = InputEnum::NOINPUT;
    InputEnum GetAIInput();
    InputEnum GetInput();
    void WaitKeyPress(char ch = 0);
};
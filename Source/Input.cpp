#include "Input.hpp"
#include <conio.h>
#include <Windows.h> //tolower()

InputEnum Input::nextAIInput = InputEnum::NOINPUT;

InputEnum Input::GetAIInput()
{
	auto ret = nextAIInput;
	nextAIInput = InputEnum::NOINPUT;
	return ret;
}

InputEnum Input::GetInput()
{
	if (kbhit()) {
		int ch = _getch();
		if (ch == 0xE0 || ch == 0) {
			ch = _getch();
			switch (ch) {
			case 72:
                return InputEnum::KEYUP;
			case 80:
                return InputEnum::KEYDOWN;
			case 75:
                return InputEnum::KEYLEFT;
			case 77:
                return InputEnum::KEYRIGHT;
			case 73: //pageup
                return InputEnum::KEYOPT1;
			case 81: //pagedown
                return InputEnum::KEYOPT0;
			case 104:
                return InputEnum::KEYHOLD;
			}
		}
		else {
			switch (tolower(ch)) {
			case 'c':
                return InputEnum::KEYHOLD;
			case 'z':
                return InputEnum::KEYLEFTROT;
			case 'x':
                return InputEnum::KEYRIGHTROT;
			case ' ':
                return InputEnum::KEYDROP;
			case 27:
                return InputEnum::KEYESC;
			case 'p':
                return InputEnum::KEYPAUSE;
			}
		}
	}
    return InputEnum::NOINPUT;
}

void Input::WaitKeyPress(char ch)
{
	if (ch == 0) {
		_getch();
		return;
	}
	while(1) {
		if (_getch() == ch) return;
	}
}
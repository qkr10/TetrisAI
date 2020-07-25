#include "Input.hpp"
#include <conio.h>
#include <Windows.h> //tolower()

InputKind Input::GetInput()
{
	if (kbhit()) {
		int ch = _getch();
		if (ch == 0xE0 || ch == 0) {
			ch = _getch();
			switch (ch) {
			case 72:
                return InputKind::UP;
			case 80:
                return InputKind::DOWN;
			case 75:
                return InputKind::LEFT;
			case 77:
                return InputKind::RIGHT;
			case 73: //pageup
                return InputKind::OPT1;
			case 81: //pagedown
                return InputKind::OPT0;
			case 104:
                return InputKind::HOLD;
			}
		}
		else {
			switch (tolower(ch)) {
			case 'c':
                return InputKind::HOLD;
			case 'z':
                return InputKind::LEFTROT;
			case 'x':
                return InputKind::RIGHTROT;
			case ' ':
                return InputKind::DROP;
			case 27:
                return InputKind::ESC;
			case 'p':
                return InputKind::PAUSE;
			}
		}
	}
    return InputKind::NONE;
}

void Input::WaitKeyPress(char ch)
{
	while(1) {
		if (_getch() == ch) return;
	}
}
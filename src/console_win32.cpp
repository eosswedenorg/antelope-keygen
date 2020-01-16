
#include <windows.h>
#include <iostream>
#include "console.h"

// WinAPI colors
#define FG_BLACK		0
#define FG_BLUE			FOREGROUND_BLUE
#define FG_GREEN		FOREGROUND_GREEN
#define FG_RED			FOREGROUND_RED
#define FG_CYAN			(FOREGROUND_GREEN | FOREGROUND_BLUE)
#define FG_YELLOW		(FOREGROUND_RED | FOREGROUND_GREEN)
#define FG_MAGENTA		(FOREGROUND_RED | FOREGROUND_BLUE)
#define FG_DARKGREY		FOREGROUND_INTENSITY
#define FG_GREY			(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define FG_LIGHTBLUE	(FOREGROUND_INTENSITY | FOREGROUND_BLUE)
#define FG_LIGHTGREEN	(FOREGROUND_INTENSITY | FOREGROUND_GREEN)
#define FG_LIGHTCYAN	(FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define FG_LIGHTRED		(FOREGROUND_INTENSITY | FOREGROUND_RED)
#define FG_LIGHTMAGENTA	(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE)
#define FG_LIGHTYELLOW	(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN)
#define FG_WHITE		(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define FG_DEFAULT		FG_GREY

namespace console {

std::ostream& reset(std::ostream& os) {

	::SetConsoleTextAttribute(::GetStdHandle(STD_OUTPUT_HANDLE), FG_DEFAULT);

	return os;
}

// Foreground
std::ostream& operator<<(std::ostream& os, const fg& obj) {

	int code;

	switch(obj._color) {
		case black : code = FG_BLACK; break;
		case red : code = FG_RED; break;
		case green : code = FG_GREEN; break;
		case blue : code = FG_BLUE; break;
		case yellow : code = FG_YELLOW; break;
		case magenta : code = FG_MAGENTA; break;
		case cyan : code = FG_CYAN; break;
		case light_grey : code = FG_GREY; break;
		case light_red : code = FG_LIGHTRED; break;
		case light_green : code = FG_LIGHTGREEN; break;
		case light_yellow : code = FG_LIGHTYELLOW; break;
		case light_blue : code = FG_LIGHTBLUE; break;
		case light_magenta : code = FG_LIGHTMAGENTA; break;
		case light_cyan : code = FG_LIGHTCYAN; break;
		case dark_grey : code = FG_DARKGREY; break;
		case white : code = FG_WHITE; break;
		case default_fg : default :
			code = FG_DEFAULT;
	}

	::SetConsoleTextAttribute(::GetStdHandle(STD_OUTPUT_HANDLE), code);

	// WinAPI does not support text attributes in the console.
	// so we ignore those.

	return os;
}

} // namespace console

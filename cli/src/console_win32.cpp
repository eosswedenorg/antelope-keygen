/**
 * MIT License
 *
 * Copyright (c) 2019-2021 EOS Sw/eden
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <windows.h>
#include <iostream>
#include "console.hpp"

namespace eoskeygen {

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

HANDLE _getNativeHandle(const std::ostream& os) {

	if (&os == &std::cout) {
    	return ::GetStdHandle(STD_OUTPUT_HANDLE);
	} else if (&os == &std::cerr) {
        return ::GetStdHandle(STD_ERROR_HANDLE);
	}
	return NULL;
}

std::ostream& reset(std::ostream& os) {

	if (isColorsSupported(os)) {
		::SetConsoleTextAttribute(_getNativeHandle(os), FG_DEFAULT);
	}
	return os;
}

// Foreground
std::ostream& operator<<(std::ostream& os, const fg& obj) {

	int code;

	if (isColorsSupported(os)) {

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

		::SetConsoleTextAttribute(_getNativeHandle(os), code);
	}

	// WinAPI does not support text attributes in the console.
	// so we ignore those.

	return os;
}

} // namespace console

} // namespace eoskeygen

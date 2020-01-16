
#include <iostream>
#include "console.h"

namespace console {

std::ostream& reset(std::ostream& os) {
	return os << "\033[0m";
}

// Foreground
std::ostream& operator<<(std::ostream& os, const fg& obj) {

	int attr;
	int code;

	switch(obj._color) {
		case black : code = 30; break;
		case red : code = 31; break;
		case green : code = 32; break;
		case yellow : code = 33; break;
		case blue : code = 34; break;
		case magenta : code = 35; break;
		case cyan : code = 36; break;
		case light_grey : code = 37; break;
		case dark_grey : code = 90; break;
		case light_red : code = 91; break;
		case light_green : code = 92; break;
		case light_yellow : code = 93; break;
		case light_blue : code = 94; break;
		case light_magenta : code = 95; break;
		case light_cyan : code = 96; break;
		case white : code = 97; break;
		case default_fg : default :
			code = 39;
	}

	switch(obj._attr) {

		case bold : attr = 1; break;
		case italic : attr = 2; break;
		case normal : default :
			attr = 0;
	}

	return os << "\033[" << attr << ";" << code << "m";
}

} // namespace console

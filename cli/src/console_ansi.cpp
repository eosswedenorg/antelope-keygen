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
#include <iostream>
#include "console.hpp"

namespace eoskeygen {

namespace console {

std::ostream& reset(std::ostream& os) {

	if (!isColorsSupported(os)) {
		return os;
	}
	return os << "\033[0m";
}

// Foreground
std::ostream& operator<<(std::ostream& os, const fg& obj) {

	int attr;
	int code;

	if (!isColorsSupported(os)) {
		return os;
	}

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

} // namespace eoskeygen

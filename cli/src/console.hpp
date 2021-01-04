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
#ifndef EOSIOKEYGEN_CONSOLE_H
#define EOSIOKEYGEN_CONSOLE_H

#include <ostream>

namespace eoskeygen {

namespace console {

	extern bool disable_color;

	// enum for all supported colors.
	enum Color {
		default_fg,
		black,
		white,
		red,
		green,
		blue,
		yellow,
		magenta,
		cyan,

		// Light colors.
		light_grey,
		light_red,
		light_green,
		light_blue,
		light_yellow,
		light_magenta,
		light_cyan,

		// Dark colors
		dark_grey
	};

	enum Attribute {
		normal,
		bold,
		italic
	};

	bool isColorsSupported(const std::ostream& os);

	// Resets all colors/attributes
	std::ostream& reset(std::ostream& os);

	// Foreground color
	// Defined as a class with overloaded "<<" operator so you can write:
	//   std::cout << fg(red) << "Text";
	class fg
	{
	public:
		fg(Color color, Attribute attribute = normal)
			: _color(color), _attr(attribute) {}

		friend std::ostream& operator<<(std::ostream& os, const fg& obj);

	protected :
		Color _color;
		Attribute _attr;
	};

} // namespace console

} // namespace eoskeygen

#endif /* EOSIOKEYGEN_CONSOLE_H */

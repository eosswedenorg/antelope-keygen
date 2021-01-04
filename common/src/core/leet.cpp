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
#include <algorithm>
#include <eoskeygen/core/leet.hpp>

namespace eoskeygen {

static bool is_l33t(char ch, char& r) {

	//                          '1', '2', '3', '4', '5', '6', '7', '8', '9'
	static char alphabet[9] = { 'l', 'z', 'e', 'a', 's', 'G', 't', 'B', 'g' };

	for(std::size_t i = 0; i < sizeof(alphabet) / sizeof(char); i++) {

		if (ch == alphabet[i]) {
			r = static_cast<char>('1' + i);
			return true;
		}
	}
	return false;
}

static void _l33t(strlist_t& list, const std::string& a, std::size_t pos) {

	// Find the next character to be replaced.
	for(std::size_t i = pos; i < a.length(); i++) {

		char ch;
		if (is_l33t(a[i], ch)) {
			// create a new string and replace the character.
			std::string b = a;
			b[i] = ch;

			// Store the new string as the result.
			list.push_back(b);

			// Perform the same algorithm for both strings
			// at the next position.
			_l33t(list, a, i + 1);
			_l33t(list, b, i + 1);
			break;
		}
	}
}

strlist_t l33twords(std::string str) {

	strlist_t list;

	// "l" is abit special and are not included in base58 so we set it to 1.
	// All other characters in "l33t" are valid.
	std::transform(str.begin(), str.end(), str.begin(), [](char c){ return c == 'l' ? '1' : c; });

	// Store the original string as the first in list.
	list.push_back(str);

	_l33t(list, str, 0);
	return list;
}

} // namespace eoskeygen

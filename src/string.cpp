/**
 * MIT License
 *
 * Copyright (c) 2019-2020 EOS Sw/eden
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
#include <cstddef>
#include <cctype>
#include <algorithm>
#include "string.h"

strlist_t strsplitwords(const std::string& str, const std::string& delim) {

	strlist_t words = strsplit(str, delim);
	std::for_each(words.begin(), words.end(), trim);
	return words;
}

strlist_t strsplit(const std::string& str, const std::string& delim) {

	strlist_t r;
	size_t s = 0, e = 0, dlen = delim.length();

	while((e = str.find(delim, s)) != std::string::npos) {
		r.push_back(str.substr(s, e - s));
		s = e + dlen;
	}

	r.push_back(str.substr(s));
	return r;
}

std::string strjoin(const strlist_t& list, const std::string& delim) {

	std::string out;

	for(const std::string& item : list) {
		if (item.length() < 1) {
			continue;
		}
		out += item + delim;
	}

	if (out.length() > 0) {
		out.erase(out.end() - delim.length());
	}

	return out;
}

std::string& strtolower(std::string& str) {
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c){ return std::tolower(c); });
	return str;
}

std::string& ltrim(std::string& str) {
	auto it = std::find_if(str.begin(), str.end(), [](char ch){ return !std::isspace(ch); });
	str.erase(str.begin(), it);
	return str;
}

std::string& rtrim(std::string& str) {
	auto it = std::find_if(str.rbegin(), str.rend(), [](char ch){ return !std::isspace(ch); });
	str.erase(it.base(), str.end());
	return str;
}

std::string& trim(std::string& str) {
	return ltrim(rtrim(str));
}

static bool _is_base58(char ch) {
	static std::string alphabet = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
	return alphabet.find(ch) != std::string::npos;
}

std::string& base58_strip(std::string &str) {
	str.erase(std::remove_if(str.begin(), str.end(), [] (std::string::value_type ch)
		{ return !_is_base58(ch); }
	), str.end());
    return str;
}

strlist_t& base58_strip(strlist_t& list) {

	std::transform(list.begin(), list.end(), list.begin(), [](std::string& str){ return base58_strip(str); });
	return list;
}

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

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
#include <eoskeygen/core/string.hpp>
#include <eoskeygen/core/strlist.hpp>

namespace eoskeygen {

strlist_t strlist::splitw(const std::string& str, const std::string& delim) {

	strlist_t words = strlist::split(str, delim);
	std::for_each(words.begin(), words.end(), trim);
	return words;
}

strlist_t strlist::split(const std::string& str, const std::string& delim) {

	strlist_t r;
	size_t s = 0, e = 0, dlen = delim.length();

	while((e = str.find(delim, s)) != std::string::npos) {
		r.push_back(str.substr(s, e - s));
		s = e + dlen;
	}

	r.push_back(str.substr(s));
	return r;
}

std::string strlist::join(const eoskeygen::strlist_t& list, const std::string& delim) {

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

strlist_t& strlist::strip(strlist_t& list, strlist_stripfunc_t fn) {

	std::transform(list.begin(), list.end(), list.begin(), fn);
	return list;
}

} // namespace eoskeygen

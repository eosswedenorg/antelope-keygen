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
#include <string>
#include "crypto/ec.h"
#include "key_search_helpers.h"
#include "key_search.h"

namespace eoskeygen {

void KeySearch::addWord(const std::string& str)
{
	std::string tmp = str;
	strtolower(tmp);
	m_words.push_back(tmp);
}

void KeySearch::addList(const strlist_t& list)
{
	for(const std::string& item : list) {
		addWord(item);
	}
}

void KeySearch::addDictionary(const Dictionary& dictionary)
{
	m_dict = dictionary;
}

const strlist_t& KeySearch::getList()
{
	return m_words;
}

void KeySearch::clear()
{
	m_words.clear();
}

void KeySearch::_search_linear(size_t n) {

	size_t count = 0;
	struct ec_keypair pair;

	while (count < n) {
		struct key_result res;
		ec_generate_key(&pair);
		if (key_contains_word(&pair, m_words, &res)) {
			key_search_result(&pair, &res, m_dict);
			count++;
		}
	}
}

void KeySearch::find(size_t num_results) {

#ifdef HAVE_THREADS
	// Only do multithread if number of threads makes sense.
	if (m_threads >= 2) {
		_search_mt(num_results);
		return;
	}
#endif /* HAVE_THREADS */

	_search_linear(num_results);
}

} // namespace eoskeygen

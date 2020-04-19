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
#include <libeosio/ec.hpp>
#include <libeosio/WIF.hpp>
#include <eoskeygen/config.h>
#include <eoskeygen/core/string.h>
#include <eoskeygen/key_search_result.h>
#include <eoskeygen/key_search.h>

namespace eoskeygen {

KeySearch::KeySearch() :
#ifdef EOSIOKEYGEN_HAVE_THREADS
	m_threads(0),
#endif
	m_callback(NULL)
{
}

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

const strlist_t& KeySearch::getList()
{
	return m_words;
}

void KeySearch::clear()
{
	m_words.clear();
}

void KeySearch::setCallback(IKeySearchResult* callback)
{
	m_callback = callback;
}

void KeySearch::_search_linear(size_t n) {

	size_t count = 0;
	struct libeosio::ec_keypair pair;

	while (count < n) {
		struct result res;
		libeosio::ec_generate_key(&pair);
		if (_contains_word(&pair, res)) {
			m_callback->onResult(&pair, res);
			count++;
		}
	}
}

void KeySearch::find(size_t num_results) {

#ifdef EOSIOKEYGEN_HAVE_THREADS
	// Only do multithread if number of threads makes sense.
	if (m_threads >= 2) {
		_search_mt(num_results);
		return;
	}
#endif /* HAVE_THREADS */

	_search_linear(num_results);
}

bool KeySearch::_contains_word(const struct libeosio::ec_keypair* key, struct result& result) {

	// skip first 3 chars, as those are always "EOS"
	std::string pubstr = libeosio::wif_pub_encode(key->pub).substr(3);
	strtolower(pubstr);

	for(auto const& w: m_words) {
		size_t p = pubstr.find(w);
		if (p != std::string::npos) {
			result.pos = p + 3;
			result.len = w.length();
			return true;
		}
	}
	return false;
}

} // namespace eoskeygen

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
#include <libantelope/ec.hpp>
#include <libantelope/WIF.hpp>
#include <eoskeygen/config.hpp>
#include <eoskeygen/core/string.hpp>
#include <eoskeygen/key_search_result.hpp>
#include <eoskeygen/key_search.hpp>

namespace antelopekeygen {

KeySearch::KeySearch() :
	m_prefix        ("EOS"),
	m_max		(0),
	m_count		(0),
#ifdef EOSIOKEYGEN_HAVE_THREADS
	m_threads	(0),
#endif
	m_callback	(NULL)
{
}

void KeySearch::setPrefix(const std::string& prefix)
{
	m_prefix = prefix;
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

void KeySearch::_search_linear()
{
	struct libantelope::ec_keypair pair;

	while (m_count < m_max) {
		struct result res;
		libantelope::ec_generate_key(&pair);
		if (_contains_word(&pair, res)) {
			m_callback->onResult(&pair, res);
			m_count++;
		}
	}
}

void KeySearch::abort()
{
	// exit find() operation by setting m_max to zero.
	m_max = 0;
}

void KeySearch::find(size_t num_results)
{
	m_count = 0;
	m_max = num_results;

#ifdef EOSIOKEYGEN_HAVE_THREADS
	// Only do multithread if number of threads makes sense.
	if (m_threads >= 2) {
		_search_mt();
		return;
	}
#endif /* HAVE_THREADS */

	_search_linear();
}

bool KeySearch::_contains_word(const struct libantelope::ec_keypair* key, struct result& result) {

	size_t prefix_len = m_prefix.length();
	std::string pubstr = libantelope::wif_pub_encode(key->pub, m_prefix).substr(prefix_len);
	strtolower(pubstr);

	for(auto const& w: m_words) {
		size_t p = pubstr.find(w);
		if (p != std::string::npos) {
			result.pos = p + prefix_len;
			result.len = w.length();
			return true;
		}
	}
	return false;
}

} // namespace antelopekeygen

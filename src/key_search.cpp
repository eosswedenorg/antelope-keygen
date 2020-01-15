
#include <iostream>
#include <string>
#include "WIF.h"
#include "key_search_helpers.h"
#include "key_search.h"

void KeySearch::addWord(const std::string& str)
{
	std::string tmp = str;
	base58_strip(tmp);
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

void KeySearch::_search_linear(size_t n) {

	size_t count = 0;
	struct ec_keypair pair;

	while (count < n) {
		std::string word;
		ec_generate_key(&pair);
		if (key_contains_word(&pair, m_words, word)) {
			key_search_result(word, &pair);
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

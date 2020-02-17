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
#ifndef EOSIOKEYGEN_KEY_SEARCH_H
#define EOSIOKEYGEN_KEY_SEARCH_H

#include "core/dictionary.h"
#include "string.h"

namespace eoskeygen {

class KeySearch
{
public :
	// Add a word to search for.
	void addWord(const std::string& str);

	// Add a list of words to search for.
	void addList(const strlist_t& list);

	void addDictionary(const Dictionary& dictionary);

	// get the list of words to search for.
	const strlist_t& getList();

	// Clears the search list.
	void clear();

#ifdef HAVE_THREADS
	// Set the number of threads to use while searching.
	void setThreadCount(size_t num);
#endif /* HAVE_THREADS */

	// Perform a search.
	void find(size_t num_results);

protected :

#ifdef HAVE_THREADS
	void _thr_proc();

	void _search_mt(size_t n);
#endif /* HAVE_THREADS */

	void _search_linear(size_t n);

protected :
	// List of words to search for.
	strlist_t m_words;

	// Dictionary to use when we find a search result.
	Dictionary m_dict;

#ifdef HAVE_THREADS
	// Number of threads to use.
	size_t m_threads;
#endif /* HAVE_THREADS */
};

} // namespace eoskeygen

#endif /* EOSIOKEYGEN_KEY_SEARCH_H */

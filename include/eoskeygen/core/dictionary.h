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
#ifndef EOSIOKEYGEN_CORE_DICTIONARY_H
#define EOSIOKEYGEN_CORE_DICTIONARY_H

#include <vector>
#include <string>
#include <map>
#include <set>

namespace eoskeygen {

class Dictionary
{
public :
    // Map that contains position and length for substrings.
	//
	// key   = position in the search string.
	// value = length of the word from this position.
	typedef std::map< size_t, size_t > search_result_t;

public :

	// Load words from file.
	bool loadFromFile(const std::string& filename);

	// Add a word to the dictionary.
	void add(const std::string& word);

	// Add words from another dictionary.
	void add(const Dictionary& dictionary);

	// Clear all words from the dictionary.
	void clear();

	// Returns true if word exists in the dictionary.
	bool contains(const std::string& word) const;

	// Searches the subject for words defined in the dictionary.
	// Returns a search_result_t with the words found in subject.
	// See search_result_t for more details.
	search_result_t search(const std::string& subject) const;

protected :

	// Words in the dictionary.
	std::set<std::string> m_words;
};

} // namespace eoskeygen

#endif /* EOSIOKEYGEN_CORE_DICTIONARY_H */

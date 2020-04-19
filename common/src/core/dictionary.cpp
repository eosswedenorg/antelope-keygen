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
#include <vector>
#include <cstdio>
#include <iterator>
#include <algorithm>
#include <eoskeygen/core/string.hpp>
#include <eoskeygen/core/dictionary.hpp>
#include <eoskeygen/core/file.hpp>

namespace eoskeygen {

struct StringContains {
    StringContains(const std::string& str, std::vector<size_t>& pos) : m_str(str), m_pos(pos) {}
    bool operator()(const std::string& w) {
		for(size_t p = m_str.find(w); p != std::string::npos; p = m_str.find(w, p+1)) {
			m_pos.push_back(p);
		}
		return !m_pos.empty();
	}
	std::string m_str;
	std::vector<size_t>& m_pos;
};

bool Dictionary::loadFromFile(const std::string& filename)
{
	strlist_t lines;

	// Clear before adding.
	clear();

	if (readLines(filename, lines)) {

		// Read each line and add to the dictionary.
		for(auto it = lines.begin(); it != lines.end(); it++) {
			add(*it);
		}
		return true;
	}
	return false;
}

void Dictionary::add(const std::string& word)
{
	// Do not insert a empty string.
	if (word.length()) {
		m_words.insert(word);
	}
}

void Dictionary::add(const Dictionary& dictionary)
{
	std::set_union(
		m_words.begin(), m_words.end(),
		dictionary.m_words.begin(), dictionary.m_words.end(),
		std::inserter(m_words, m_words.begin())
	);
}

const std::set<std::string>& Dictionary::getWords() const
{
	return m_words;
}

void Dictionary::clear()
{
	m_words.clear();
}

bool Dictionary::contains(const std::string& word) const
{
	return m_words.find(word) != m_words.cend();
}

Dictionary::search_result_t Dictionary::search(const std::string& subject) const
{
	search_result_t res;

	std::vector<size_t> pos;
	StringContains pred(subject, pos);

	// Find all words.
	for(auto it = std::find_if(m_words.begin(), m_words.end(), pred);
		it != m_words.end();
		it = std::find_if(++it, m_words.end(), pred)) {

		// Go through all found positions.
		for (auto it2 = pos.begin(); it2 != pos.end(); it2++) {

			// Insert
			auto rit = res.find(*it2);
			if (rit == res.end()) {
				res.emplace(*it2, it->length());
			}
			// Update length if it's longer then the previous we found.
			else if (rit->second < it->length()) {
				rit->second = it->length();
			}
		}

		// Clear positions
		pos.clear();
	}

	return res;
}

} // namespace eoskeygen


#include <iostream>
#include <cstdio>
#include <algorithm>
#include "../string.h"
#include "dictionary.h"

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
	FILE *fd;
	char buf[1024];

	fd = fopen(filename.c_str(), "r");
	if (!fd) {
		return false;
	}

	// Clear before adding.
	clear();

	// Read each line and add to the dictionary.
	while(fgets(buf, sizeof(buf), fd) != NULL) {
		std::string word(buf);
		add(trim(word));
	}

	fclose(fd);
	return true;
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

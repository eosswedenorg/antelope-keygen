
#include <iostream>
#include <string>
#include "WIF.h"
#include "key_search_helpers.h"
#include "key_search.h"

void key_search_n(const strlist_t& word_list, size_t n) {

	size_t count = 0;
	struct ec_keypair pair;

	while (count < n) {
		std::string word;
		ec_generate_key(&pair);
		if (key_contains_word(&pair, word_list, word)) {
			key_search_result(word, &pair);
			count++;
		}
	}
}

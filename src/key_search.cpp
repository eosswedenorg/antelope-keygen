
#include <iostream>
#include <string>
#include "WIF.h"
#include "key_search_helpers.h"
#include "key_search.h"

bool key_search(struct ec_keypair* key, std::string& word, const strlist_t& word_list) {

	std::string pubstr;

	ec_generate_key(key);
	pubstr = wif_pub_encode(key->pub);
	strtolower(pubstr);

	for(auto const& w: word_list) {
		if (pubstr.find(w) != std::string::npos) {
			word = w;
			return true;
		}
	}
	return false;
}

void key_search_n(const strlist_t& word_list, size_t n) {

	size_t count = 0;
	struct ec_keypair pair;

	while (count < n) {
		std::string word;
		if (key_search(&pair, word, word_list)) {
			key_search_result(word, &pair);
			count++;
		}
	}
}

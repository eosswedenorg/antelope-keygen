
#include <iostream>
#include "WIF.h"
#include "key_search_helpers.h"

void key_search_result(const std::string& word, const struct ec_keypair* pair) {

	std::cout << "----" << std::endl;
	std::cout << "Found: " << word << std::endl;
	wif_print_key(pair);
}

bool key_contains_word(const struct ec_keypair* key, const strlist_t& word_list, std::string& word) {

	std::string pubstr = wif_pub_encode(key->pub);
	strtolower(pubstr);

	for(auto const& w: word_list) {
		if (pubstr.find(w) != std::string::npos) {
			word = w;
			return true;
		}
	}
	return false;
}

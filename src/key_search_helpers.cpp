
#include <iostream>
#include "WIF.h"
#include "console.h"
#include "key_search_helpers.h"

void key_search_result(const struct ec_keypair* key, const struct key_result* result) {

	std::string pub = wif_pub_encode(key->pub);
	std::string word = pub.substr(result->pos, result->len);

	std::cout << "----" << std::endl;
	std::cout << "Found: " << word << std::endl;

	std::cout << "Public: "
		<< pub.substr(0, result->pos)
		<< console::fg(console::red, console::bold) << word << console::reset
		<< pub.substr(result->pos + result->len)
		<< std::endl;

	std::cout << "Private: " << wif_priv_encode(key->secret) << std::endl;
}

bool key_contains_word(const struct ec_keypair* key, const strlist_t& word_list, struct key_result *result) {

	// skip first 3 chars, as those are always "EOS"
	std::string pubstr = wif_pub_encode(key->pub).substr(3);
	strtolower(pubstr);

	for(auto const& w: word_list) {
		size_t p = pubstr.find(w);
		if (p != std::string::npos) {
			result->pos = p + 3;
			result->len = w.length();
			return true;
		}
	}
	return false;
}


#include <iostream>
#include "WIF.h"
#include "key_search_helpers.h"

void key_search_result(const std::string& word, const struct ec_keypair* pair) {

	std::cout << "----" << std::endl;
	std::cout << "Found: " << word << std::endl;
	wif_print_key(pair);
}

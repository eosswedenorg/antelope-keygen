
#include <iostream>
#include <string>
#include "ec.h"
#include "WIF.h"
#include "key_search.h"

void key_search(strlist_t word_list, size_t n) {

	size_t count = 0;
	struct ec_keypair pair;

	while (count < n) {
		std::string pubstr;
		ec_generate_key(&pair);
		pubstr = wif_pub_encode(pair.pub);
		strtolower(pubstr);

		for(auto const& word: word_list) {
			if (pubstr.find(word) != std::string::npos) {
				std::cout << "----" << std::endl;
				std::cout << "Found: " << word << std::endl;
				wif_print_key(&pair);
				count++;
			}
		}
	}
}

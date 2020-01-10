
#ifdef HAVE_THREADS
#include <mutex>
#endif /* HAVE_THREADS */
#include <iostream>
#include <string>
#include "ec.h"
#include "WIF.h"
#include "key_search.h"

#ifdef HAVE_THREADS
// Guards result output.
std::mutex search_mutex;
#endif /* HAVE_THREADS */

static void key_result(const std::string& word, const struct ec_keypair* pair) {

#ifdef HAVE_THREADS
	// Guard output with mutex, so we don't get interrupted mid write.
	const std::lock_guard<std::mutex> lock(search_mutex);
#endif /* HAVE_THREADS */

	std::cout << "----" << std::endl;
	std::cout << "Found: " << word << std::endl;
	wif_print_key(pair);
}

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
			key_result(word, &pair);
			count++;
		}
	}
}


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
				key_result(word, &pair);
				count++;
			}
		}
	}
}

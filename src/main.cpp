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
#include <algorithm>
#include <iostream>
#include <cstring>
#include "string.h"
#include "base58.h"
#include "WIF.h"
#include "ec.h"
#include "key_search.h"

#ifdef HAVE_THREADS
#include <thread>
#include <vector>

// Minium number of threads.
#define MIN_THREADS 2

// Number of threads to use.
static int n_threads = MIN_THREADS;

#define search_func thread_search
static void thread_search(const strlist_t& words, int n) {

	// create n_threads - 1 as we use main process also.
	std::vector<std::thread> t(n_threads - 1);
	// divide the number of results for all threads.
	int d = n / n_threads;
	// Also calculate the reminder (will be assigned to the main thread)
	int m = n % n_threads;

	// Launch threads.
	for(int i = 0; i < t.size(); i++) {
		t[i] = std::thread(key_search, words, d);
	}

	// Use main thread for 1 search
	key_search(words, d + m);

	// Wait for all threads to compelete.
	for(int i = 0; i < t.size(); i++) {
		t[i].join();
	}
}
#else
#define search_func key_search
#endif

void cmd_search(int argc, char **argv) {

	int n = 100;
	std::string search(argv[0]);
	strlist_t words = strsplitwords(strtolower(search));

	if (argc > 1) {
		n = atoi(argv[1]);
		if (n < 1) {
			n = 1;
		}
	}

#ifdef HAVE_THREADS
	if (argc > 2) {
		n_threads = atoi(argv[2]);
		// Make sure we never go under min threads.
		if (n_threads < MIN_THREADS) {
			n_threads = MIN_THREADS;
		}
	}
# endif /* HAVE_THREADS */

	std::cout << "Searching for " << n
	<< " keys containing: " << search
#ifdef HAVE_THREADS
	<< ", Using: " << n_threads << " threads"
#endif /* HAVE_THREADS */
	<< std::endl;

	search_func(words, n);
}

void usage(const char *name) {

	std::cout << name
		<< " [ --help|-h ] [ search <word_list> [ <count:100> ]"
#ifdef HAVE_THREADS
		<< " [ <threads:2> ]"
#endif /* HAVE_THREADS */
		" ]" << std::endl << std::endl;

	std::cout << " - Output one EOSIO key pair if no arguments are given" << std::endl << std::endl;

	std::cout << " search: " << std::endl
			  << "  performs a search, finding <count> public keys "
			  << "containing one or more words from <word_list>."
#ifdef HAVE_THREADS
			  << std::endl << "  <threads> specify the number of parallel threads to use."
#endif /* HAVE_THREADS */
			  << std::endl;
}

int main(int argc, char **argv) {

	// search <word_list> [ <count> ]
	if (argc > 1) {

		if (!strcmp(argv[1], "-h") && !strcmp(argv[1], "--help")) {
			usage(argv[0]);
			return 0;
		}

		if (!strcmp(argv[1], "search")) {
			int n = 100;
			std::string search;
			strlist_t words;

			if (argc < 3) {
				std::cout << "You must specify a word list." << std::endl;
				usage(argv[0]);
				return 1;
			}

			cmd_search(argc - 2, &argv[2]);
		} else {
			std::cout << "Unrecogniced command: " << argv[1] << std::endl;
			usage(argv[0]);
			return 1;
		}
	} else {
		struct ec_keypair pair;
		ec_generate_key(&pair);
		wif_print_key(&pair);
	}

	return 0;
}

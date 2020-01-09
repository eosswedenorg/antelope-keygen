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
static int n_threads;

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

int main(int argc, char **argv) {

	// search <word_list> [ <count> ]
	if (argc > 2 && !strcmp(argv[1], "search")) {
		int n = 100;
		std::string search(argv[2]);
		strlist_t words = strsplitwords(strtolower(search));

		if (argc > 3) {
			n = atoi(argv[3]);
		}

#ifdef HAVE_THREADS
		if (argc > 4) {
			n_threads = atoi(argv[4]);
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

	} else {
		struct ec_keypair pair;
		ec_generate_key(&pair);
		wif_print_key(&pair);
	}

	return 0;
}

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
#ifdef HAVE_THREADS
#include <thread>
#endif /* HAVE_THREADS */
#include <algorithm>
#include <iostream>
#include <cstring>
#include "string.h"
#include "base58.h"
#include "WIF.h"
#include "ec.h"
#include "key_search.h"

// Command line options.
bool option_l33t = false;

#ifdef HAVE_THREADS
#define n_thread_decl int n_threads = std::thread::hardware_concurrency()
#define n_thread_argv 				\
	if (argc > 2) { 				\
		n_threads = atoi(argv[2]);	\
		if (n_threads < 2) {		\
			n_threads = 2;			\
		}							\
	}
#define n_thread_outp << ", Using: " << n_threads << " threads"
#define call_search key_search_mt(words, n, n_threads)
#else
#define n_thread_decl
#define n_thread_argv
#define n_thread_outp
#define call_search key_search(words, n)
#endif /* HAVE_THREADS */

void cmd_search(int argc, char **argv) {

	int n = 100;
	n_thread_decl;
	std::string search(argv[0]);
	strlist_t words;

	if (option_l33t) {
		strlist_t tmp = strsplitwords(search);
		for(int i = 0; i < tmp.size(); i++) {
			strlist_t list = l33twords(base58_strip(tmp[i]));
			words.reserve(words.size() + list.size());
			words.insert(words.end(), list.begin(), list.end());
		}
	} else {
		words = strsplitwords(strtolower(search));
		base58_strip(words);
	}

	if (argc > 1) {
		n = atoi(argv[1]);
		if (n < 1) {
			n = 1;
		}
	}

	n_thread_argv;

	std::cout << "Searching for " << n
		<< " keys containing: " << strjoin(words, ",")
		n_thread_outp
		<< std::endl;

	call_search;
}

void usage(const char *name) {

	std::cout << name
		<< " [ --help|-h ] [ search <word_list> [ <count:10> ]"
#ifdef HAVE_THREADS
		<< " [ <threads:2> ]"
#endif /* HAVE_THREADS */
		" ]" << std::endl << std::endl;

	std::cout << " - Output one EOSIO key pair if no arguments are given" << std::endl << std::endl;

	std::cout << " search: " << std::endl
			  << "  performs a search, finding <count> public keys "
			  << "containing one or more words from <word_list> (separated with ',')."
#ifdef HAVE_THREADS
			  << std::endl << "  <threads> specify the number of parallel threads to use."
#endif /* HAVE_THREADS */
			  << std::endl;
}

int main(int argc, char **argv) {

	// search <word_list> [ <count> ]
	if (argc > 1) {

		if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
			usage(argv[0]);
			return 0;
		}

		if (!strcmp(argv[1], "--l33t")) {
			option_l33t = true;
			argc--;
			argv = &argv[1];
		}

		if (!strcmp(argv[1], "search")) {
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

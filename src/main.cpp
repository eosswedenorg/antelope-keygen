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
#include <iostream>
#include <cstring>
#include "string.h"
#include "WIF.h"
#include "ec.h"
#include "key_search.h"

// Command line options.
bool option_l33t = false;

#ifdef HAVE_THREADS
int option_num_threads = std::thread::hardware_concurrency();
#endif /* HAVE_THREADS */

void cmd_search(int argc, char **argv) {

	int n = 10;
	std::string input(argv[0]);
	KeySearch ks;

	if (option_l33t) {
		strlist_t tmp = strsplitwords(input);
		for(int i = 0; i < tmp.size(); i++) {
			ks.addList(l33twords(tmp[i]));
		}
	} else {
		ks.addList(strsplitwords(input));
	}

	if (argc > 1) {
		n = atoi(argv[1]);
		if (n < 1) {
			n = 1;
		}
	}

#ifdef HAVE_THREADS
	ks.setThreadCount(option_num_threads);
#endif /* HAVE_THREADS */

	std::cout << "Searching for " << n
		<< " keys containing: " << strjoin(ks.getList(), ",")
#ifdef HAVE_THREADS
		<< ", Using: " << option_num_threads << " threads"
#endif /* HAVE_THREADS */
		<< std::endl;

	ks.find(n);
}

void usage(const char *name) {

	std::cout << name
		<< " [ -h | --help | search [ --l33t"
#ifdef HAVE_THREADS
		<< " | --threads=<num>"
#endif /* HAVE_THREADS */
		<< " ] <word_list> [ <count:10> ] ]"
		<< std::endl << std::endl;

	std::cout << " - Output one EOSIO key pair if no arguments are given" << std::endl << std::endl;

	// Options
	std::cout
		<< " Options:" << std::endl
		<< "   -h, --help     Shows this help text."
		<< std::endl
		<< std::endl;

	std::cout << " search: " << std::endl
			  << "  performs a search, finding <count> public keys containing" << std::endl
			  << "  one or more words from <word_list> (separated with ',')."
			  << std::endl << std::endl
			  << "  --l33t: Takes each word in <word_list> and find all l33tspeak" << std::endl
			  << "          combinations of that word and uses the new list for the search."
#ifdef HAVE_THREADS
			  << std::endl << std::endl
			  << "  --threads=<num>: Use <num> of parallel threads for searching." << std::endl
			  << "                   Default is what the operating system recomend."
#endif /* HAVE_THREADS */
			  << std::endl;
}

int main(int argc, char **argv) {

	// current position in argv
	// when parsing command line.
	int p = 0;

	if (argc > 1) {
		p = 1;

		if (!strcmp(argv[p], "-h") || !strcmp(argv[p], "--help")) {
			usage(argv[0]);
			return 0;
		}

		if (!strcmp(argv[p], "search")) {
			p++;

			if (p < argc && !strcmp(argv[p], "--l33t")) {
				option_l33t = true;
				p++;
			}

#ifdef HAVE_THREADS
			if (p < argc && !memcmp(argv[p], "--threads=", 10)) {
				option_num_threads = atoi(argv[p] + 10);
				if (option_num_threads < 2) {
					option_num_threads = 2;
				}
				p++;
			}
#endif /* HAVE_THREADS */

			if (argc <= p) {
				std::cout << "You must specify a word list." << std::endl;
				usage(argv[0]);
				return 1;
			}

			// Pass the rest of argv, argc
			cmd_search(argc - p, &argv[p]);
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

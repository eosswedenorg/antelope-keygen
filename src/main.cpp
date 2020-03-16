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
#include "config.h"
#include "core/file.h"
#include "core/dictionary.h"
#include "core/string.h"
#include "crypto/base58.h"
#include "crypto/ec.h"
#include "crypto/WIF.h"
#include "console.h"
#include "key_search.h"
#include "benchmark.h"

// Command line options.
bool option_l33t = false;

#ifdef HAVE_THREADS
int option_num_threads = std::thread::hardware_concurrency();
#endif /* HAVE_THREADS */

int cmd_search(const eoskeygen::strlist_t& words, const eoskeygen::Dictionary& dict, int count) {

	eoskeygen::KeySearch ks;

	ks.addDictionary(dict);

	for(auto it = words.begin(); it != words.end(); it++) {
		size_t p = eoskeygen::is_base58(*it);
		if (p != std::string::npos) {
			std::cerr << "The word '"
				<< *it << "' contains an invalid non-base58 character '"
				<< (*it)[p] << "'" << std::endl;
			return 1;
		}
	}

	if (option_l33t) {
		for(std::size_t i = 0; i < words.size(); i++) {
			ks.addList(eoskeygen::l33twords(words[i]));
		}
	} else {
		ks.addList(words);
	}

#ifdef HAVE_THREADS
	ks.setThreadCount(option_num_threads);
#endif /* HAVE_THREADS */

	std::cout << "Searching for " << count
		<< " keys containing: " << eoskeygen::strjoin(ks.getList(), ",")
#ifdef HAVE_THREADS
		<< ", Using: " << option_num_threads << " threads"
#endif /* HAVE_THREADS */
		<< std::endl;

	ks.find(count);

	return 0;
}

void usage(const char *name) {

	std::cout << name
		<< " [ -h | --help | -v | search [ -m | --l33t"
#ifdef HAVE_THREADS
		<< " | --threads=<num>"
#endif /* HAVE_THREADS */
		<< " | --dict=<file> ... "
		<< " | --lang=<value> ... ] <word_list>|file:<filename> [ <count:10> ]"
		<< " | benchmark [ <num:1000> ]"
		<< " ]"
		<< std::endl << std::endl;

	std::cout << " - Output one EOSIO key pair if no arguments are given" << std::endl << std::endl;

	// Options
	std::cout
		<< " Options:" << std::endl
		<< "   -h, --help     Shows this help text."
		<< std::endl << std::endl
		<< "   -v             Shows version."
		<< std::endl << std::endl;

	std::cout << " search: " << std::endl
			  << "  performs a search, finding <count> public keys containing" << std::endl
			  << "  one or more words from <word_list> (separated with ',')." << std::endl
			  << std::endl
			  << "  Instead of a list it is possible to specify a file with words" << std::endl
			  << "  (separated with newline '\\n') using file:<filename>"
			  << std::endl << std::endl
			  << "  -m: Monochrome, disables all color output."
			  << std::endl << std::endl
			  << "  --l33t: Takes each word in <word_list> and find all l33tspeak" << std::endl
			  << "          combinations of that word and uses the new list for the search."
#ifdef HAVE_THREADS
			  << std::endl << std::endl
			  << "  --threads=<num>: Use <num> of parallel threads for searching." << std::endl
			  << "                   Default is what the operating system recomend."
#endif /* HAVE_THREADS */
			  << std::endl << std::endl
			  << "  --dict=<file>: Use words found in <file> (separated by newline) to" << std::endl
			  << "                 highlight words in the keys found (note that the words in this" << std::endl
			  << "                 file are not used for search. only for highlight output)." << std::endl
			  << "                 There can be more then one --dict flag. In that case contents" << std::endl
			  << "                 of all files are merged into one dictionary." << std::endl
			  << std::endl << std::endl
			  << "  --lang=<value>: Same as --dict but will use <value>" << std::endl
			  << "                  to find a file in " << CONFIG_SHARE_FULL_PATH << "/dict." << std::endl
			  << "                  There can be more then one --lang flag. In that case contents" << std::endl
			  << "                  of all files are merged into one dictionary." << std::endl
			  << std::endl;

	std::cout << " Benchmark: " << std::endl
			  << "  performs a benchmark test, generating <num> keys and measuring the time." << std::endl
			  << std::endl;
}

void cmd_benchmark(size_t num_keys) {

	struct eoskeygen::benchmark_result res;

	std::cout << "Benchmark: Generating "
		<< num_keys << " keys" << std::endl;

	eoskeygen::benchmark(num_keys, &res);

	std::cout << "Result: Took " << res.sec << " seconds, "
		<< res.kps << " keys per second." << std::endl;
}

int main(int argc, char **argv) {

	// current position in argv
	// when parsing command line.
	int p = 1;

	// No args, just print a key.
	if (argc <= 1) {
		struct eoskeygen::ec_keypair pair;
		eoskeygen::ec_generate_key(&pair);
		eoskeygen::wif_print_key(&pair);
		return 0;
	}

	if (!strcmp(argv[p], "-h") || !strcmp(argv[p], "--help")) {
		usage(argv[0]);
		return 0;
	}

	if (!strcmp(argv[p], "-v")) {
		std::cout << PROGRAM_NAME << " v" << PROGRAM_VERSION << std::endl;
		return 0;
	}

	if (!strcmp(argv[p], "search")) {

		int count = 10;
		eoskeygen::strlist_t words;
		eoskeygen::Dictionary dict;

		while(p++ < argc - 1) {
			if (!strcmp(argv[p], "-m")) {
				eoskeygen::console::disable_color = true;
			} else if (!strcmp(argv[p], "--l33t")) {
				option_l33t = true;
			} else if (!memcmp(argv[p], "--threads=", 10)) {
#ifdef HAVE_THREADS
				option_num_threads = atoi(argv[p] + 10);
				if (option_num_threads < 2) {
					std::cerr << "NOTICE: Number of threads less than 2 does not make sense."
						<< " So eosio-keygen will use 2." << std::endl;
					option_num_threads = 2;
				}
#else
				// Even if we dont have threads. we consume the flag.
				// otherwise we might break scripts. Print a nice message instead.
				std::cerr << "NOTICE: eosio-keygen is not compiled with"
					<< " thread support. this option is ignored." << std::endl;
#endif /* HAVE_THREADS */
			}
			// Dictionary.
			else if (!memcmp(argv[p], "--dict=", 7)) {
				eoskeygen::Dictionary d;
				std::string filename(argv[p] + 7);

				if (d.loadFromFile(filename)) {
					dict.add(d);
				} else {
					std::cerr << "Could not load dictionary from file: " << filename << std::endl;
				}
			}
			// Language (dictionary, but we find the file in <CONFIG_SHARE_FULL_PATH>/dict/<lang>)
			else if (!memcmp(argv[p], "--lang=", 7)) {
				eoskeygen::Dictionary d;
				std::string lang(argv[p] + 7);
				std::string filename(std::string(CONFIG_SHARE_FULL_PATH) + "/dict/" + lang);

				if (d.loadFromFile(filename)) {
					dict.add(d);
				} else {
					std::cerr << "Could not load language " << lang << " (" << filename << ")" << std::endl;
				}
			}
			// Error out on any flag we don't support.
			else if (argv[p][0] == '-') {
				std::cerr << "Unrecognized flag: " << argv[p] << std::endl;
				usage(argv[0]);
				return 0;
			}
			// wordlist and count
			else if (words.size() < 1) {
				std::string arg = std::string(argv[p]);
				if (arg.rfind("file:", 0) == 0) {
					std::string filename = arg.substr(5);
					if (!eoskeygen::readLines(filename, words)) {
						std::cerr << "Could not read file: " << filename << std::endl;
						return 0;
					}

					if (words.size() < 1) {
						std::cerr << filename << " did not contain any words" << std::endl;
						return 0;
					}
				}
				// List
				else {
					words = eoskeygen::strsplitwords(arg);
				}

				if (p + 1 < argc) {
					count = atoi(argv[++p]);
					if (count < 1) {
						count = 1;
					}
				}
			}
		}

		if (words.size() < 1) {
			std::cerr << "You must specify a word list." << std::endl;
			usage(argv[0]);
			return 1;
		}

		return cmd_search(words, dict, count);
	}
	// Benchmark
	else if (!strcmp(argv[p], "benchmark")) {
		int num_keys = 1000;

		if (++p < argc) {
			num_keys = atoi(argv[p]);
			if (num_keys < 1) {
				num_keys = 1;
			}
		}

		cmd_benchmark(num_keys);
	} else {
		std::cerr << "Unrecogniced command: " << argv[1] << std::endl;
		usage(argv[0]);
		return 1;
	}

	return 0;
}

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

static void search(std::string words, size_t n) {

	size_t count = 0;
	struct ec_keypair pair;
	std::vector<std::string> word_list;

	std::cout << "Searching for " << n << " keys containing: " << words << std::endl;

	word_list = strsplitwords(strtolower(words));

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

int main(int argc, char **argv) {

	// search <word_list> [ <count> ]
	if (argc > 2 && !strcmp(argv[1], "search")) {
		int n = 100;
		if (argc > 3) {
			n = atoi(argv[3]);
		}
		search(argv[2], n);
	} else {
		struct ec_keypair pair;
		ec_generate_key(&pair);
		wif_print_key(&pair);
	}

	return 0;
}

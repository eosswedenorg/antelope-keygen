/**
 * MIT License
 *
 * Copyright (c) 2019-2021 EOS Sw/eden
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
#include <iostream>
#include <cstring>
#include <CLI11/CLI11.hpp>
#include <libantelope/base58.hpp>
#include <libantelope/ec.hpp>
#include <libantelope/WIF.hpp>
#include <eoskeygen/config.hpp>
#include <eoskeygen/core/file.hpp>
#include <eoskeygen/core/string.hpp>
#include <eoskeygen/core/dictionary.hpp>
#include <eoskeygen/core/leet.hpp>
#include <eoskeygen/key_search.hpp>
#include "cli_key_search_result.hpp"
#include "console.hpp"
#include "benchmark.hpp"
#include "config.hpp"

// Command line options.
bool option_l33t = false;
libantelope::wif_codec_t key_codec;

#ifdef EOSIOKEYGEN_HAVE_THREADS
size_t option_num_threads;
#endif /* EOSIOKEYGEN_HAVE_THREADS */

class CustomFormatter : public CLI::Formatter {
public:

	std::string make_usage(const CLI::App *app, std::string name) const
	{
		std::stringstream out;

		out << std::endl << CLI::Formatter::make_usage(app, name)
			<< std::endl
			<< "Outputs one EOSIO key pair if no subcommand is given"
			<< std::endl;

		return out.str();
	}
};

int cmd_search(const antelopekeygen::strlist_t& words, const antelopekeygen::Dictionary& dict, int count) {

	antelopekeygen::KeySearch ks;
	antelopekeygen::CliKeySearchResult rs(dict, key_codec);

	ks.setPrefix(key_codec.pub);
	ks.setCallback(&rs);

	for(auto it = words.begin(); it != words.end(); it++) {
		size_t p = libantelope::is_base58(*it);
		if (p != std::string::npos) {
			std::cerr << "The word '"
				<< *it << "' contains an invalid non-base58 character '"
				<< (*it)[p] << "'" << std::endl;
			return 1;
		}
	}

	if (option_l33t) {
		for(std::size_t i = 0; i < words.size(); i++) {
			ks.addList(antelopekeygen::l33twords(words[i]));
		}
	} else {
		ks.addList(words);
	}

#ifdef EOSIOKEYGEN_HAVE_THREADS
	ks.setThreadCount(option_num_threads);
#endif /* EOSIOKEYGEN_HAVE_THREADS */

	std::cout << "Searching for " << count
		<< " keys containing: " << antelopekeygen::strlist::join(ks.getList(), ",")
#ifdef EOSIOKEYGEN_HAVE_THREADS
		<< ", Using: " << ks.getThreadCount() << " threads"
#endif /* EOSIOKEYGEN_HAVE_THREADS */
		<< std::endl;

	ks.find(count);

	return 0;
}

void cmd_benchmark(size_t num_keys) {

	struct antelopekeygen::benchmark_result res;

	std::cout << "Benchmark: Generating "
		<< num_keys << " keys" << std::endl;

	antelopekeygen::benchmark(num_keys, &res);

	std::cout << "Result: Took " << res.sec << " seconds, "
		<< res.kps << " keys per second." << std::endl;
}

int main(int argc, char **argv) {

	CLI::App cmd("Keygenerator for EOSIO", PROGRAM_NAME);
	std::vector<std::string> dict_list;
	std::vector<std::string> lang_list;
	std::string search_words;
	std::string key_format;
	int search_count;
	size_t bench_count;
	int rc = 0;

	libantelope::ec_init();

	CLI::Option* version = cmd.add_flag("-v,--version", "Show version");
	cmd.add_option("--format", key_format, "valid values: K1, fio, legacy")->default_val("K1");

	// Search
	CLI::App* search_cmd = cmd.add_subcommand("search",
		"performs a search, finding <count> public keys containing "
		"one or more words from <word_list> (separated with \",\")");
	CLI::Option* monocrome = search_cmd->add_flag("-m", "Monochrome, disables all color output.");

	search_cmd->add_flag("--l33t", option_l33t, "Takes each word in <word_list> and find all l33tspeak"
		" combinations of that word and uses the new list for the search.");

#ifdef EOSIOKEYGEN_HAVE_THREADS
	search_cmd->add_option("--threads", option_num_threads,
		"Use <num> of parallel threads for searching.\n"
		"Default is what the operating system recomends.")
		->default_val(antelopekeygen::KeySearch::max_threads());

#endif /* EOSIOKEYGEN_HAVE_THREADS */

	search_cmd->add_option("--dict", dict_list, "");
	search_cmd->add_option("--lang", lang_list, "");
	search_cmd->add_option("word_list", search_words,
		"one or more words (separated with \",\")\n\n"
		"Instead of a list it is possible to specify a file with words\n"
		"(separated with newline '\\n') using file:<filename>")->required();
	search_cmd->add_option("count", search_count, "Number of keys to search for before the program terminates.")->default_val(10);

	// Benchmark
	CLI::App* bench_cmd = cmd.add_subcommand("benchmark", "performs a benchmark test, "
		"generating <num> keys and measuring the time.");
	bench_cmd->add_option("count", bench_count, "")->default_val(1000);

	// Parse command line.
	cmd.formatter(std::make_shared<CustomFormatter>());

	CLI11_PARSE(cmd, argc, argv);

	if (*version) {
		std::cout << PROGRAM_NAME << ": v" << PROGRAM_VERSION << std::endl;
		goto end;
	}

	if (key_format == "fio") {
		key_codec = libantelope::wif_create_legacy_codec("FIO");
	} else if (key_format == "legacy") {
		key_codec = libantelope::WIF_CODEC_LEG;
	} else if (key_format == "K1") {
		key_codec = libantelope::WIF_CODEC_K1;
	} else {
		std::cerr << "invalid key format: " << key_format << std::endl;
		goto end;
	}

	if (search_cmd->parsed()) {
		antelopekeygen::strlist_t words;
		antelopekeygen::Dictionary dict;

		if (*monocrome) {
			antelopekeygen::console::disable_color = true;
		}

		for (auto item : dict_list) {
			antelopekeygen::Dictionary d;

			if (d.loadFromFile(item)) {
				dict.add(d);
			} else {
				std::cerr << "Could not load dictionary from file: " << item << std::endl;
			}
		}

		for (auto item : lang_list) {
			antelopekeygen::Dictionary d;
			std::string filename(CONFIG_SHARE_FULL_PATH "/dicts/" + item);

			if (d.loadFromFile(filename)) {
				dict.add(d);
			} else {
				std::cerr << "Could not load dictionary from language file: " << filename << std::endl;
			}
		}

		if (search_words.rfind("file:", 0) == 0) {
			std::string filename = search_words.substr(5);
			if (!antelopekeygen::readLines(filename, words)) {
				std::cerr << "Could not read file: " << filename << std::endl;
				goto end;
			}

			if (words.size() < 1) {
				std::cerr << filename << " did not contain any words" << std::endl;
				goto end;
			}
		} else {
			words = antelopekeygen::strlist::splitw(search_words);
		}

		rc = cmd_search(words, dict, search_count);
		goto end;

	} else if (bench_cmd->parsed()) {
		cmd_benchmark(bench_count);
	}
	// No subcommand given, just generate and print a keypair.
	else {
		struct libantelope::ec_keypair pair;
		libantelope::ec_generate_key(&pair);
		libantelope::wif_print_key(&pair, key_codec);
		goto end;
	}

end: 	libantelope::ec_shutdown();
	return rc;
}

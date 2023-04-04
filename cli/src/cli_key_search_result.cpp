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
#include <string>
#include <iostream>
#include <libeosio/WIF.hpp>
#include <eoskeygen/core/dictionary.hpp>
#include "console.hpp"
#include "cli_key_search_result.hpp"

namespace eoskeygen {

static size_t highlight(console::Color color, const std::string& str, size_t pos, size_t len) {

	std::cout << console::fg(color, console::bold)
		<< str.substr(pos, len)
		<< console::reset;
	return len;
}

CliKeySearchResult::CliKeySearchResult(const Dictionary& dict, const libeosio::wif_codec_t& codec) :
m_dict (dict),
m_codec (codec)
{
}

void CliKeySearchResult::onResult(const struct libeosio::ec_keypair* key, const struct KeySearch::result& result) {

	std::string pub = libeosio::wif_pub_encode(key->pub, m_codec.pub);
	Dictionary::search_result_t dict_res = m_dict.search(pub);
	int pub_prefix_len = (int) m_codec.pub.length();

	std::cout << "----" << std::endl;
	std::cout << "Found: " << pub.substr(result.pos, result.len) << std::endl;

	std::cout << "Public: " << m_codec.pub;
	for(size_t i = pub_prefix_len; i < pub.length(); ) {

		if (i == result.pos) {
			i += highlight(console::red, pub, result.pos, result.len);
			continue;
		}

		auto p = dict_res.find(i);
		if (p != dict_res.end()) {
			i += highlight(console::blue, pub, p->first, p->second);
			continue;
		}

		std::cout << pub[i++];
	}

	std::cout << std::endl
		<< "Private: " << libeosio::wif_priv_encode(key->secret, m_codec.pvt) << std::endl;
}

} // namespace eoskeygen

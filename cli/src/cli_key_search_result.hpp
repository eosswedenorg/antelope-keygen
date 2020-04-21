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
#ifndef EOSIOKEYGEN_KEY_SEARCH_HELPERS_H
#define EOSIOKEYGEN_KEY_SEARCH_HELPERS_H

#include <libeosio/types.hpp>
#include <eoskeygen/core/string.hpp>
#include <eoskeygen/key_search.hpp>
#include <eoskeygen/key_search_result.hpp>

namespace eoskeygen {

class Dictionary;

class CliKeySearchResult : public IKeySearchResult
{
public:
	CliKeySearchResult(const Dictionary& dict);

	virtual void onResult(const struct libeosio::ec_keypair* key, const struct KeySearch::result& result);

protected :

	const Dictionary& m_dict;
};

} // namespace eoskeygen

#endif /* EOSIOKEYGEN_KEY_SEARCH_HELPERS_H */

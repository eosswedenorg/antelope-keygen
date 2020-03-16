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
#ifndef EOSIOKEYGEN_CRYPTO_BASE58_H
#define EOSIOKEYGEN_CRYPTO_BASE58_H

#include <string>
#include <vector>

namespace eoskeygen {

std::string base58_encode(const std::string& str);
std::string base58_encode(const std::vector<unsigned char>& vch);
std::string base58_encode(const unsigned char* pbegin, const unsigned char* pend);

bool is_base58(char ch);

// Returns std::string::npos if the string contains only base58 characters
// Otherwise the position of the first non base58 character is returned.
size_t is_base58(const std::string& str);

std::string& base58_strip(std::string& str);

} //namespace eoskeygen

#endif /* EOSIOKEYGEN_CRYPTO_BASE58_H */

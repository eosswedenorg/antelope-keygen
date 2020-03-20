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
#ifndef EOSIOKEYGEN_CRYPTO_EOS_CHECKSUM_H
#define EOSIOKEYGEN_CRYPTO_EOS_CHECKSUM_H

#include <cstdint>
#include <cstring>
#include <array>
#include <eoskeygen/crypto/hash.h>

namespace eoskeygen {

#define CHECKSUM_SIZE 4

typedef std::array<unsigned char, CHECKSUM_SIZE> checksum_t;

template <typename T, T* (*F)(const unsigned char *, std::size_t, T*)>
inline checksum_t checksum(const unsigned char* data, std::size_t len) {
	checksum_t crc;
	T hash;

	F(data, len, &hash);
	std::memcpy(crc.data(), &hash, crc.size());
	return crc;
}

#define checksum_sha256 checksum<sha256_t, sha256>
#define checksum_sha256d checksum<sha256_t, sha256d>
#define checksum_ripemd160 checksum<ripemd160_t, ripemd160>

} // namespace eoskeygen

#endif /* EOSIOKEYGEN_CHECKSUM_H */

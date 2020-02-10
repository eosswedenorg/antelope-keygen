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
#include <openssl/sha.h>
#include <openssl/ripemd.h>
#include <cstring>
#include "checksum.h"

namespace eoskeygen {

inline void sha256d(const unsigned char *data, std::size_t len, unsigned char *out) {
	SHA256(data, len, out);
	SHA256(out, 32, out);
}

#define checksum_impl(name, func) \
	checksum_t checksum_##name(const unsigned char *data, std::size_t len) { 	\
																				\
		checksum_t crc;															\
		unsigned char hash[32];													\
																				\
		func(data, len, hash);													\
																				\
		std::memcpy(crc.data(), hash, crc.size());								\
		return crc;																\
	}


checksum_impl(sha256d, sha256d)
checksum_impl(ripemd160, RIPEMD160)

} // namespace eosio-keygen

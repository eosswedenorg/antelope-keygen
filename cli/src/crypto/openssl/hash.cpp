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
#include "../hash.h"

namespace eoskeygen {

sha256_t* sha256(const unsigned char *data, std::size_t len, sha256_t* out) {
	return (sha256_t *) SHA256(data, len, out->data);
}

sha256_t* sha256d(const unsigned char *data, std::size_t len, sha256_t* out) {
	SHA256(data, len, out->data);
	return (sha256_t *) SHA256(data, 32, out->data);
}

ripemd160_t* ripemd160(const unsigned char *data, std::size_t len, ripemd160_t* out) {
	return (ripemd160_t *) RIPEMD160(data, len, out->data);
}

} // namespace eoskeygen

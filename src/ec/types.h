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
#ifndef EC_TYPES_H
#define EC_TYPES_H

#include <array>

namespace eoskeygen {

#define EC_PRIVKEY_SIZE 32

/*
 * Compressed format!
 * z||x, where byte z specifies which (of the 2) solutions of the quadratic equation y is.
 * Each cordinate is 32 bytes.
 */
#define EC_PUBKEY_SIZE (32 + 1)

typedef std::array<unsigned char, EC_PRIVKEY_SIZE> ec_privkey_t;
typedef std::array<unsigned char, EC_PUBKEY_SIZE> ec_pubkey_t;

struct ec_keypair {
	ec_privkey_t secret;
	ec_pubkey_t pub;
};

} // namespace eoskeygen

#endif /* EC_TYPES_H */

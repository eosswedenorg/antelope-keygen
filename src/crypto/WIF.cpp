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
#include <iostream>
#include <string.h>
#include "base58.h"
#include "checksum.h"
#include "WIF.h"

namespace eoskeygen {

#define PRIV_KEY_PREFIX 0x80 /* 0x80 for "Bitcoin mainnet". Always used by EOS. */

std::string wif_priv_encode(ec_privkey_t priv) {

	checksum_t check;
	// 1 byte extra for prefix.
	unsigned char buf[1 + EC_PRIVKEY_SIZE + CHECKSUM_SIZE] = { PRIV_KEY_PREFIX };

	memcpy(buf + 1, priv.data(), priv.size());

	// Checksum
	check = checksum_sha256d(buf, 1 + EC_PRIVKEY_SIZE);
	memcpy(buf + 1 + EC_PRIVKEY_SIZE, check.data(), check.size());

	return base58_encode(buf, buf + sizeof(buf));
}

std::string wif_pub_encode(ec_pubkey_t pub) {

	checksum_t check = checksum_ripemd160(pub.data(), pub.size());
	unsigned char buf[EC_PUBKEY_SIZE + CHECKSUM_SIZE];

	memcpy(buf, pub.data(), pub.size());
	memcpy(buf + EC_PUBKEY_SIZE, check.data(), check.size());

	return "EOS" + base58_encode(buf, buf + sizeof(buf));
}

void wif_print_key(const struct ec_keypair *key) {

	std::cout << "Public: " << wif_pub_encode(key->pub) << std::endl;
	std::cout << "Private: " << wif_priv_encode(key->secret) << std::endl;
}

} // namespace eoskeygen

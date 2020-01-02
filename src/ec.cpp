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
#include <openssl/ec.h>
#include <openssl/bn.h>
#include <openssl/hmac.h>
#include "ec.h"

static int ec_generate_pair(unsigned char *priv, unsigned char *pub) {

	int ret = -1;
	EC_KEY *k;
	BN_CTX *ctx;

	// Create BIGNUM context.
	ctx = BN_CTX_new();
	if (ctx == NULL) {
		return -1;
	}

	// Construct curve.
	k = EC_KEY_new_by_curve_name(NID_secp256k1);
	if (k == NULL) {
		goto fail1;
	}

	// Generate new key pair.
	if (EC_KEY_generate_key(k) != 1)  {
		goto fail2;
	}

	// Copy private key to binary format.
	EC_KEY_priv2oct(k, priv, EC_PRIVKEY_SIZE);

	// Copy public key key
	EC_POINT_point2oct(EC_KEY_get0_group(k),
		EC_KEY_get0_public_key(k), POINT_CONVERSION_COMPRESSED,
	   	pub, EC_PUBKEY_SIZE, ctx);

	ret = 0;
fail2:
	EC_KEY_free(k);
fail1:
	BN_CTX_free(ctx);
	return ret;
}

int ec_generate_key(struct ec_keypair *pair) {

	return ec_generate_pair(pair->secret.data(), pair->pub.data());
}

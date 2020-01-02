#!/usr/bin/env bash

BASE_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

# Build openssl
. "${BASE_DIR}/emscripten_openssl.sh" "openssl"

#  Configure cmake
emconfigure cmake \
      -DCMAKE_BUILD_TYPE=Release \
	  -DOPENSSL_ROOT_DIR="openssl" \
	  -DOPENSSL_CRYPTO_LIBRARY="openssl/libcrypto.a" \
	  -DOPENSSL_SSL_LIBRARY="openssl/libssl.a" \
      -G "Unix Makefiles" $@ .. \
	  -DCMAKE_EXE_LINKER_FLAGS="--bind"

emmake make

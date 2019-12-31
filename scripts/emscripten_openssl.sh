#!/usr/bin/env bash
# Build OpenSSL

BUILD_DIR=$1
OPENSSL_VERSION=1.1.1d

if [ ! -d "$BUILD_DIR" ]; then
	mkdir -p "$BUILD_DIR"

	wget https://www.openssl.org/source/openssl-${OPENSSL_VERSION}.tar.gz
	tar xf openssl-${OPENSSL_VERSION}.tar.gz -C "$BUILD_DIR" --strip 1
	rm -f openssl-${OPENSSL_VERSION}.tar.gz
fi

pushd "$BUILD_DIR"

emconfigure ./Configure gcc -no-tests -no-asm -static -no-sock \
	-no-afalgeng -DOPENSSL_SYS_NETWARE -DSIG_DFL=0 -DSIG_IGN=0 \
	-DHAVE_FORK=0 -DOPENSSL_NO_AFALGENG=1 --with-rand-seed=getrandom

sed -i 's|^CROSS_COMPILE.*$|CROSS_COMPILE=|g' Makefile
#sed -i -E 's|^CFLAGS=(.*)$|CFLAGS=\1 -D__STDC_NO_ATOMICS__=1|g' Makefile

emmake make -j 12 build_generated libssl.a libcrypto.a
popd

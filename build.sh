#!/bin/bash

BASE_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

mkdir build 2> /dev/null
pushd build > /dev/null

# Emscripten WASM build.
if [ $# -gt 0 ] && [ "$1" = "em-wasm" ]; then
	shift 1
	. ${BASE_DIR}/scripts/emscripten.sh $@
else
	cmake $@ ..
	make -B
fi

popd > /dev/null

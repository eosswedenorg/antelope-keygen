#!/bin/bash

mkdir build 2> /dev/null
pushd build > /dev/null

function usage() {
	echo "Usage: ${0##*/} [ -h ] [ -t Debug|Release|RelWithDebInfo|MinSizeRel ] [ --disable-threads ] [ --force-ansi ]"
	exit 1
}

options=$(getopt -n "${0##*/}" -o ht: --long disable-threads --long force-ansi -- "$@")

[ $? -eq 0 ] || usage

eval set -- "$options"

ARGS=""
while true; do

	case "${1}" in
		-t)
			shift
			[[ ! "$1" =~ ^(Debug|Release|RelWithDebInfo|MinSizeRel)$ ]] && {
	            echo "Incorrect type '$1' provided"
	            usage
	        }
			ARGS="${ARGS} -DCMAKE_BUILD_TYPE=${1}"
			;;
		--disable-threads)
			ARGS="${ARGS} -DUSE_THREADS=OFF" ;;
		--force-ansi)
			ARGS="${ARGS} -DFORCE_ANSI=ON" ;;
		-h) usage ;;
		--) shift
			break
			;;
	esac
	shift
done

cmake $ARGS ..
make -B

popd > /dev/null

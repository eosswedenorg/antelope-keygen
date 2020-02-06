#!/bin/bash

mkdir build 2> /dev/null
pushd build > /dev/null

function usage() {
	echo "Usage: ${0##*/} [ -h ] [ -t Debug|Release|RelWithDebInfo|MinSizeRel ] [ --disable-threads ] [ --force-ansi ]"
	exit 1
}

options=$(getopt -n "${0##*/}" -o lht: --long disable-threads --long force-ansi -- "$@")

[ $? -eq 0 ] || usage

eval set -- "$options"

ONLY_CONFIG=0
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
		-l)
			ARGS="${ARGS} -LH"
			ONLY_CONFIG=1 ;;
		-h) usage ;;
		--) shift
			break
			;;
	esac
	shift
done

cmake $ARGS ..
if [ ${ONLY_CONFIG} -eq 0 ]; then
	make -B
fi

popd > /dev/null

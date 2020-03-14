#!/bin/bash

function usage() {
	echo "Usage: ${0##*/} [ -h|--help ] [ -t|--type Debug|Release|RelWithDebInfo|MinSizeRel ] [ --disable-threads ] [ --force-ansi ]"
	exit 1
}

options=$(getopt -n "${0##*/}" -o "lht:" -l "help,type:,disable-threads,force-ansi" -- "$@")

[ $? -eq 0 ] || usage

eval set -- "$options"

ONLY_CONFIG=0
ARGS=""
while true; do

	case $1 in
	-t|--type)
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
	-h|--help)	usage ;;
	--)	shift
		break
		;;
	esac
	shift
done

cmake -B build $ARGS .
if [ ${ONLY_CONFIG} -eq 0 ]; then
	cmake --build build --clean-first --target
fi

#!/bin/bash

function usage() {
	echo "Usage: ${0##*/} [ -h|--help ] [ --no-cli ] [ --gui] [ -t|--type Debug|Release|RelWithDebInfo|MinSizeRel ] [--pkg-type nsis|deb|zip|tgz] [ --disable-threads ] [ --force-ansi ]"
	exit 1
}

options=$(getopt -n "${0##*/}" -o "lht:" -l "help,no-cli,gui,type:,pkg-type:,disable-threads,force-ansi" -- "$@")

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
	--pkg-type)
		shift
		[[ ! "$1" =~ ^(nsis|deb|zip|tgz)$ ]] && {
		    echo "Incorrect package type '$1' provided"
		    usage
		}
		TARGET="package"
		ARGS="${ARGS} -DCPACK_GENERATOR=${1^^}"
		;;
	--no-cli)
		ARGS="${ARGS} -DCOMPONENT_CLI=OFF" ;;
	--gui)
		ARGS="${ARGS} -DCOMPONENT_GUI=ON" ;;
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

# Remove cache first
rm build/CMakeCache.txt 2> /dev/null

cmake -B build $ARGS .
if [ ${ONLY_CONFIG} -eq 0 ]; then
	cmake --build build --clean-first --target ${TARGET}
fi

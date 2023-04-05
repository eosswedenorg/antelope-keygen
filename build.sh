#!/bin/bash

function usage() {
	echo "Usage: ${0##*/} [ -h|--help ] [ --cli|--no-cli ] [ --gui|--no-gui] [ -t|--type Debug|Release|RelWithDebInfo|MinSizeRel ] [ --libeosio=<path> ] [ --pkg-type nsis|deb|zip|tgz ] [ --disable-threads ] [ --force-ansi ]"
	exit 1
}

options=$(getopt -n "${0##*/}" -o "lht:" -l "help,cli,no-cli,gui,no-gui,type:,libeosio:,pkg-type:,disable-threads,force-ansi" -- "$@")

[ $? -eq 0 ] || usage

eval set -- "$options"

TARGET="all"
ONLY_CONFIG=0
ARGS=""
BUILD_ARGS="--clean-first"
while true; do

	case $1 in
	-t|--type)
		shift
		[[ ! "$1" =~ ^(Debug|Release|RelWithDebInfo|MinSizeRel)$ ]] && {
		    echo "Incorrect type '$1' provided"
		    usage
		}
		ARGS="${ARGS} -DCMAKE_BUILD_TYPE=${1}"
		BUILD_ARGS="${BUILD_ARGS} --config ${1}"
		;;
	--pkg-type)
		shift
		[[ ! "$1" =~ ^(nsis|deb|rpm|zip|tgz)$ ]] && {
		    echo "Incorrect package type '$1' provided"
		    usage
		}
		TARGET="package"
		ARGS="${ARGS} -DCPACK_GENERATOR=${1^^}"
		;;
	--libeosio)
		shift
		ARGS="${ARGS} -DLIBEOSIO_SOURCE_DIR=${1}"
		;;
	--cli)
		ARGS="${ARGS} -DCOMPONENT_CLI=ON" ;;
	--no-cli)
		ARGS="${ARGS} -DCOMPONENT_CLI=OFF" ;;
	--gui)
		ARGS="${ARGS} -DCOMPONENT_GUI=ON" ;;
	--no-gui)
		ARGS="${ARGS} -DCOMPONENT_GUI=OFF" ;;
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
	cmake --build build ${BUILD_ARGS} --target ${TARGET}
fi

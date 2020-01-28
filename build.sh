#!/bin/bash

mkdir build 2> /dev/null
pushd build > /dev/null

function usage() {
	echo "Usage: ${0##*/} [ -h ] [ -t Debug|Release|RelWithDebInfo|MinSizeRel ]"
	exit 1
}

ARGS=""
while getopts ":h?t:" opt; do

	case "${opt}" in
		t) 	ARGS="${ARGS} -DCMAKE_BUILD_TYPE=${OPTARG}" ;;
		h)  usage ;;
		:) 	echo "Error: -${OPTARG} requires a value"
			exit 1
			;;
	esac
done
shift $((OPTIND-1))

cmake $ARGS $@ ..
make -B

popd > /dev/null

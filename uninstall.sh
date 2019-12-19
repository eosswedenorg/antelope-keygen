#!/bin/bash

MANIFEST=build/install_manifest.txt

if [ ! -f ${MANIFEST} ]; then
	echo "Missing manifest: ${MANIFEST}" > /dev/stderr
	exit 1
fi

for file in $(cat ${MANIFEST}); do
	rm -f $file
done

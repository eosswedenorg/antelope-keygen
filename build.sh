#!/bin/bash

mkdir build 2> /dev/null
pushd build > /dev/null
cmake .. $@
make -B
popd > /dev/null

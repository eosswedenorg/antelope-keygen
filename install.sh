#!/bin/bash

pushd build > /dev/null
make install $@
popd > /dev/null

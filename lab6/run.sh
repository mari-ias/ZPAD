#!/bin/bash
set -e

if [ ! -f build/lab6 ]; then
    echo "Build not found. Run ./build.sh first."
    exit 1
fi

cd build
./lab6
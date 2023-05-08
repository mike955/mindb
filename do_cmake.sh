#!/usr/bin/env bash
set -x
# git submodule update --init --recursive
if test -e build; then
    echo 'build dir already exists; rm -rf build and re-run'
    exit 1
fi

if type ccache > /dev/null 2>&1 ; then
    echo "enabling ccache"
    ARGS="$ARGS -DWITH_CCACHE=ON"
fi

if [[ ! "$ARGS $@" =~ "-DCMAKE_BUILD_TYPE" ]]; then
    ARGS="$ARGS -DCMAKE_BUILD_TYPE=Debug"
fi

mkdir build
cd build
cmake $ARGS "$@" .. || exit 1

echo done.

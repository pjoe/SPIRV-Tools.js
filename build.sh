#!/bin/bash
set -ex

NUM_CORES=$(nproc)

build() { 
    type=$1
    shift
    args=$@
    mkdir -p build/$type
    pushd build/$type
    echo $args
    emcmake cmake \
        -DCMAKE_BUILD_TYPE=Release \
        $args \
        ../../SPIRV-Tools
    emmake make -j $(( $NUM_CORES )) SPIRV-Tools

    echo Building js interface
    emcc \
        --bind \
        -I../../SPIRV-Tools/include \
        -std=c++11 \
        ../../src/spirv-tools.cpp \
        source/libSPIRV-Tools.a \
        -o spirv-tools.js \
        -s MODULARIZE \
        -Oz

    popd
    mkdir -p dist/$type
    cp src/spirv-tools.d.ts dist/$type/

    cp build/$type/spirv-tools.js dist/$type/
    gzip -9 -k -f dist/$type/spirv-tools.js
    brotli     -f dist/$type/spirv-tools.js
    if [ -e build/$type/spirv-tools.wasm ] ; then
       cp build/$type/spirv-tools.wasm dist/$type/
       gzip -9 -k -f dist/$type/spirv-tools.wasm
       brotli     -f dist/$type/spirv-tools.wasm
    fi
}

if [ ! -d SPIRV-Tools/external/spirv-headers ] ; then
    echo "Fetching SPIRV-headers"
    git clone https://github.com/KhronosGroup/SPIRV-Headers.git SPIRV-Tools/external/spirv-headers
fi

echo Building ${BASH_REMATCH[1]}
build web\
    -DSPIRV_COLOR_TERMINAL=OFF\
    -DSPIRV_SKIP_TESTS=ON\
    -DSPIRV_SKIP_EXECUTABLES=ON

wc -c dist/*/*

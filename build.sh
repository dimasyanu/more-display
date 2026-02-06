#!/bin/bash

rm -rf build
mkdir -p build/debug
/usr/bin/cmake \
    --no-warn-unused-cli \
    -DCMAKE_BUILD_TYPE:STRING=Debug \
    -DCMAKE_MAKE_PROGRAM:STRING=/usr/bin/g++ \
    -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
    -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/gcc \
    -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++ \
    -S /home/yanoo/projects/personal/more-display \
    -B /home/yanoo/projects/personal/more-display/build/debug \
    -G Ninja
#!/bin/bash
cmake -B build -DSysCAll_BUILD_TESTS=ON
cmake --build build
./build/sysCall_test
./build/sysCall
cd ..
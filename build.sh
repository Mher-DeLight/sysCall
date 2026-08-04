#!/bin/bash
cmake -B build -DSysCAll_BUILD_TESTS=ON
cmake --build build
./build/SysCAll_test
./build/SysCAll

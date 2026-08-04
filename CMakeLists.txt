cmake_minimum_required(VERSION 3.16)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
project(SysCAll VERSION 0.1.0 LANGUAGES CXX)
option(SysCAll_BUILD_TESTS "Build BBS-Mallocator tests" OFF)
option(SysCAll_RUN_BENCHMARKS "Run BBS-MALLOCATOR benchmarks" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_executable(SysCAll src/main.cpp src/SysCAll.cpp src/Tokenizer.cpp)
target_include_directories(SysCAll
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

if(SysCAll_BUILD_TESTS)
  include(CTest)
  enable_testing()

  add_executable(SysCAll_test
        tests/test.cpp
    )

  add_test(NAME RunBBSMallocatorTests COMMAND SysCAll_test)
endif()

if(SysCAll_RUN_BENCHMARKS)
  add_executable(SysCAll_benchmark
    benchmarks/benchmark.cpp
  )
endif()


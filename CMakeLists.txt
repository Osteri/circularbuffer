cmake_minimum_required(VERSION 2.8)

project(circularbuffer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ignore IDE macro
set(void ${QT_QMAKE_EXECUTABLE})

enable_testing()

# Google Test 
add_subdirectory(vendor/google/googletest/googletest)
add_subdirectory(test)

# Google Benchmark (compiles only in release)
#set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Suppressing benchmark's tests" FORCE)
#add_subdirectory(vendor/google/benchmark)
#add_subdirectory(bench)

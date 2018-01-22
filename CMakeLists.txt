cmake_minimum_required(VERSION 2.8)

project(circularbuffer)
add_definitions(-std=gnu++17) #todo: change to MSVC compatible

enable_testing()
add_subdirectory(tests)

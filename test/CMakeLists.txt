cmake_minimum_required(VERSION 2.8)

# add source/include files
aux_source_directory(. SOURCES)
include_directories(. .. INCLUDES)

set(CMAKE_CXX_FLAGS "-Wall -Wno-error -fno-omit-frame-pointer -fsanitize=address")
set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address")

add_executable("${PROJECT_NAME}-test" ${SOURCES})
target_link_libraries("${PROJECT_NAME}-test" gtest)
#add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

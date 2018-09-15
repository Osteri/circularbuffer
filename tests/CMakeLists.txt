project(circularbuffer-tests)

set(CMAKE_CXX_FLAGS "-Wall -Wno-error -fno-omit-frame-pointer -fsanitize=address")
set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address")

add_executable(${PROJECT_NAME} "test.cpp")
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

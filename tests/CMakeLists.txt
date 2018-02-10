project(circularbuffer-tests)
add_executable(${PROJECT_NAME} "test.cpp")
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

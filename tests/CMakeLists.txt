project(circularbuffer-tests)
add_executable(${PROJECT_NAME} "test2.cpp")
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

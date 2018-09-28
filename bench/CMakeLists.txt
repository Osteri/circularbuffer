cmake_minimum_required(VERSION 2.8)

# add source/include files
aux_source_directory(. SOURCES)
include_directories(. .. INCLUDES)

add_executable("${PROJECT_NAME}-bench" ${SOURCES})
target_link_libraries("${PROJECT_NAME}-bench" benchmark)

project(cuneiform C CXX)
cmake_minimum_required(VERSION 2.4.7)
if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})

if(PROJECT_BINARY_DIR STREQUAL PROJECT_SOURCE_DIR)
   message(FATAL_ERROR "In-tree build attempt detected, aborting. Set your build dir outside your source dir and try again.")
endif()

add_subdirectory(datafiles)
add_subdirectory(cuneiform_src)
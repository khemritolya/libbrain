cmake_minimum_required(VERSION 3.12)
project(libbrain)

set(CMAKE_CXX_STANDARD 17)

# Note well person reading this:
# These gcc compiler flags are your friend!
SET(CMAKE_CXX_FLAGS "-g -O2")

add_library(libbrain STATIC libbrain.h)

set_target_properties(libbrain PROPERTIES LINKER_LANGUAGE CXX)
enable_testing()
add_subdirectory(tests)
cmake_minimum_required(VERSION 3.14)
project(libbrain)

set(CMAKE_CXX_STANDARD 17)

# Note well person reading this:
# These gcc compiler flags are your friend!
SET(CMAKE_CXX_FLAGS "-g -O2")

add_library(libbrain libbrain.h)

set_target_properties(libbrain PROPERTIES LINKER_LANGUAGE CXX)

set(libbrain_tests Statictest)
add_executable(libbrain_tests tests/run_tests.cpp)
target_link_libraries(libbrain_tests libbrain)
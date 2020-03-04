add_executable(libbrain_tests run_tests.cpp)
target_include_directories(libbrain_tests PUBLIC ..)
target_link_libraries(libbrain_tests libbrain)
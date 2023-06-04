cmake_minimum_required(VERSION 3.26)
project(Simulatore VERSION 0.1.0)

include(CTest)
enable_testing()

add_subdirectory(test)
add_subdirectory(src)


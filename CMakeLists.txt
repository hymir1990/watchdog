cmake_minimum_required(VERSION 3.16)
project(perfgov)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
set(CMAKE_VERBOSE_MAKEFILE ON)

add_executable(performance_gov service.cpp)


cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 23)

project(
    aoc_23_6
    LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3")


add_executable(main main.cpp)

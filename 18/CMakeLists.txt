cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 20)

project(
    aoc_23_18
    LANGUAGES CXX
)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3")


add_executable(main main.cpp )

target_link_libraries(main PUBLIC )


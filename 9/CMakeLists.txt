cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 23)


project(
    aoc_23_9
    LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3")


add_executable(main main.cpp)

target_link_libraries(main PUBLIC)


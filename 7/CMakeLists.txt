cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 23)

find_package(Catch2 3 REQUIRED)

project(
    aoc_23_7
    LANGUAGES CXX
)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3")

# These tests can use the Catch2-provided main
add_executable(
    tests
    test_datastructures.cpp
    test_parser.cpp
)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

add_executable(main main.cpp )

target_link_libraries(main PUBLIC )


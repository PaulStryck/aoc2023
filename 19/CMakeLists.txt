cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 23)

project(
    aoc_23_19
    LANGUAGES CXX
)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3")

add_subdirectory(grammar)

add_executable(main main.cpp parser.cpp)

target_link_libraries(main PUBLIC grammar)

target_include_directories(
    main PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/grammar"
)


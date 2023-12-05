cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 23)

find_package(Catch2 3 REQUIRED)
find_package(Boost REQUIRED )

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )


project(
    aoc_23_5
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
TARGET_LINK_LIBRARIES(tests LINK_PUBLIC ${Boost_LIBRARIES} )

add_executable(main main.cpp )

target_link_libraries(main PUBLIC )
target_include_directories(
    main PUBLIC
    "${PROJECT_BINARY_DIR}"
)

TARGET_LINK_LIBRARIES(main LINK_PUBLIC ${Boost_LIBRARIES})

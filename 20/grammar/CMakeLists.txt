cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 23)

project(
    grammar
    LANGUAGES CXX
)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3")

find_package(BISON)
find_package(FLEX)

BISON_TARGET(
    Parser
    grammar.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
    COMPILE_FLAGS "-t -pgrammar_"
    DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.h
)

FLEX_TARGET(
    Scanner
    grammar.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp
    COMPILE_FLAGS "-Pgrammar_"
)
ADD_FLEX_BISON_DEPENDENCY(Scanner Parser)

add_library(grammar
    ${BISON_Parser_OUTPUTS}
    ${FLEX_Scanner_OUTPUTS}
    Absyn.C
)

target_include_directories(grammar PRIVATE ".")

cmake_minimum_required(VERSION 3.5.1)

project(parser_bison C CXX)


execute_process(
    COMMAND bison -ld -o parser.tab.cc parser.y
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

add_definitions(-DUSE_LEX_WITH_YACC)

if (MSVC)
    include_directories(${CMAKE_CURRENT_LIST_DIR}/../../lexer/flex/include)
else()
    add_definitions(-Wno-writable-strings)
endif()

add_executable(parser_bison
    "../../lexer/flex/lex.yy.cc"
    "../ast.h"
    "../ast.cpp"
    "parser.y"
    "parser.tab.hh"
    "parser.tab.cc"
    "parser.h"
    "parser.cpp"
    "main.cpp"
)

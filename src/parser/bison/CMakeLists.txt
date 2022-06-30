cmake_minimum_required(VERSION 3.5.1)

project(parser_bison C CXX)


execute_process(
    COMMAND bison -ld -o parser.tab.cc parser.y
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

add_definitions(-DUSE_LEX_WITH_YACC)

if (MSVC)
    include_directories(${CMAKE_CURRENT_LIST_DIR}/../../lexer/flex/include)
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

if (MSVC)
    target_compile_definitions(parser_bison
        PRIVATE -D_CRT_NONSTDC_NO_DEPRECATE )
    target_compile_options(parser_bison
        PRIVATE /wd5033 /wd4267 /wd4244 /wd4065)
else()
    target_compile_options(parser_bison
        PRIVATE -Wno-writable-strings)
endif()

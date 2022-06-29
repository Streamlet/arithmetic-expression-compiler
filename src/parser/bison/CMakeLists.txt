cmake_minimum_required(VERSION 3.5.1)

project(parser_bison C CXX)

execute_process(
    COMMAND bison -ldt parser.y
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

add_definitions(-DUSE_LEX_WITH_YACC)

add_executable(parser_bison
    "../../lexer/flex/lex.yy.c"
    "parser.y"
    "parser.tab.h"
    "parser.tab.c"
    "parser.h"
    "parser.cpp"
    "main.cpp"
)

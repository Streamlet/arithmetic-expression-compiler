cmake_minimum_required(VERSION 3.5.1)

project(lexer_flex C CXX)

execute_process(
    COMMAND flex lex.l
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

add_executable(lexer_flex
    "../token.h"
    "../token.cpp"
    "lex.l"
    "lex.yy.c"
    "lex.h"
    "lex.cpp"
    "main.cpp"
)
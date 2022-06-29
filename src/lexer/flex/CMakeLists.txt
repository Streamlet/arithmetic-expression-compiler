cmake_minimum_required(VERSION 3.5.1)

project(lexer_flex C CXX)

execute_process(
    COMMAND flex -L -o lex.yy.cc lex.l
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

add_executable(lexer_flex
    "../token_type.h"
    "../token.h"
    "../token.cpp"
    "lex.l"
    "lex.yy.cc"
    "lex.h"
    "lex.cpp"
    "main.cpp"
)

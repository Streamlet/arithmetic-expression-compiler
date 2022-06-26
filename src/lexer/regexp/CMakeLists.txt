cmake_minimum_required(VERSION 3.5.1)

project(lexer_regexp C CXX)

add_executable(lexer_regexp
    "../token.h"
    "../token.cpp"
    "lex.h"
    "lex.cpp"
    "main.cpp"
)

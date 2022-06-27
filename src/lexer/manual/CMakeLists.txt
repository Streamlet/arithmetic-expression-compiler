cmake_minimum_required(VERSION 3.5.1)

project(lexer_manual C CXX)

add_executable(lexer_manual
    "../token.h"
    "../token_v.h"
    "../token.cpp"
    "lex.h"
    "lex.cpp"
    "main.cpp"
)

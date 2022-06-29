cmake_minimum_required(VERSION 3.5.1)

project(lexer_flex C CXX)

execute_process(
    COMMAND flex -L -olex.yy.cc lex.l
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

if (MSVC)
    include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
endif()

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

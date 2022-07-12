cmake_minimum_required(VERSION 3.5.1)

project(lexer_manual C CXX)

add_executable(lexer_manual
    "../token_type.h"
    "../token.h"
    "../token.cpp"
    "lex.h"
    "lex.cpp"
    "main.cpp"
)

if (MSVC)
    target_compile_definitions(lexer_manual
        PRIVATE -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS)
endif()

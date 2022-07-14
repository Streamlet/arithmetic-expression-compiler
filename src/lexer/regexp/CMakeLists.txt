cmake_minimum_required(VERSION 3.5.1)

project(lexer_regexp C CXX)

add_executable(lexer_regexp
    "../token_type.h"
    "../token.h"
    "../token.cpp"
    "lex.h"
    "lex.cpp"
    "main.cpp"
)

if (MSVC)
    target_compile_definitions(lexer_regexp
        PRIVATE -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS)
endif()

install(TARGETS lexer_regexp
        RUNTIME DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../../../bin)

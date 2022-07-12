cmake_minimum_required(VERSION 3.5.1)

project(parser_manual C CXX)


add_executable(parser_manual
    "../../lexer/token_type.h"
    "../../lexer/token.h"
    "../../lexer/token.cpp"
    "../../lexer/manual/lex.h"
    "../../lexer/manual/lex.cpp"
    "../ast.h"
    "../ast.cpp"
    "parser.h"
    "parser.cpp"
    "main.cpp"
)

if (MSVC)
    target_compile_definitions(parser_manual
        PRIVATE -D_CRT_NONSTDC_NO_DEPRECATE )
    target_compile_options(parser_manual
        PRIVATE /wd5033 /wd4267 /wd4244 /wd4065)
else()
    target_compile_options(parser_manual
        PRIVATE -Wno-writable-strings)
endif()

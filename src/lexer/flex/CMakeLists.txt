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

if (MSVC)
    target_compile_definitions(lexer_flex
        PRIVATE -D_CRT_NONSTDC_NO_DEPRECATE )
    target_compile_options(lexer_flex
        PRIVATE /wd5033 /wd4267 /wd4244 /wd4065)
endif()

install(TARGETS lexer_flex
        RUNTIME DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../../../bin)

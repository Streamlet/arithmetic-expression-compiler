#pragma once

enum Token {
    NUM = 1,
    ADD,
    SUB,
    MUL,
    DIV,
    EXP,
    MOD,
    LPAREN,
    RPAREN,
    FUNC,
    COMMA,
};

extern const char *TOKEN_NAMES[];

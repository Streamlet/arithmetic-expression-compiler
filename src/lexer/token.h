#pragma once

enum Token {
    ADD = 1,
    SUB,
    MUL,
    DIV,
    EXP,
    MOD,
    LPAREN,
    RPAREN,
    COMMA,
    NUM,
    FUNC,
};

extern const char *TOKEN_NAMES[];
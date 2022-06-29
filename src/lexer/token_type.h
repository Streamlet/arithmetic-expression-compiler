#pragma once

enum yytokentype {
    YYEMPTY = -2,
    YYEOF = 0,
    YYerror = 256,
    YYUNDEF,
    ADD,
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
typedef enum yytokentype yytoken_kind_t;

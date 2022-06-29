#pragma once

#include "../token.h"

class Lexer {
public:
    Lexer(const char *yy_str);
    ~Lexer();

    Token next();
    const char *text();

private:
    void *yy_ctx;
};

std::vector<Token> lex(const char *s);

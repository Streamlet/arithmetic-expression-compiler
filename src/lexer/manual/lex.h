#pragma once

#include "../token_v.h"

class Lexer {
public:
    Lexer(const char *yy_str);
    ~Lexer();

    TokenV next();
    const char *text();

private:
    void *yy_ctx;
};

std::vector<TokenV> lex(const char *s);

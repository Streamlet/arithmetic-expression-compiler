#pragma once

#include "../token.h"
#include <string>
#include <vector>

struct TokenV {
    Token token;
    double dval;
    std::string str;
};

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
void print(std::vector<TokenV> token_list);

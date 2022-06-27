#include "lex.h"
#include <stdio.h>
#include <string.h>

extern "C" void *yy_init_ctx(const char *yy_str);
extern "C" void yy_free_ctx(void *yy_ctx);
extern "C" int yylex();
extern "C" char *yy_get_current_text();
extern "C" double yy_get_dval();
extern "C" int yy_get_strlen();
extern "C" const char *yy_get_str();

Lexer::Lexer(const char *yy_str) {
    yy_ctx = yy_init_ctx(yy_str);
}
Lexer::~Lexer() {
    yy_free_ctx(yy_ctx);
}

TokenV Lexer::next() {
    Token t = (Token)yylex();
    TokenV v{t};
    switch (t) {
    case NUM:
        v.dval = yy_get_dval();
        break;
    case FUNC:
        v.str.assign(yy_get_str(), (size_t)yy_get_strlen());
        break;
    default:
        break;
    }
    return std::move(v);
}

const char * Lexer::text() {
    return yy_get_current_text();
}

std::vector<TokenV> lex(const char *s) {
    std::vector<TokenV> r;
    Lexer l(s);
    while (true) {
        TokenV t = l.next();
        if (t.token > 0) {
            r.push_back(std::move(t));
        } else {
            if (t.token < 0) {
                printf("unrecognized charactor: %s\n", l.text());
            }
            break;
        }
    }
    return r;
}

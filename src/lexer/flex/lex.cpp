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

Token Lexer::next() {
    yytoken_kind_t k = (yytoken_kind_t)yylex();
    Token t{k};
    switch (k) {
    case NUM:
        t.dval = yy_get_dval();
        break;
    case FUNC:
        t.str.assign(yy_get_str(), (size_t)yy_get_strlen());
        break;
    default:
        break;
    }
    return std::move(t);
}

const char * Lexer::text() {
    return yy_get_current_text();
}

std::vector<Token> lex(const char *s) {
    std::vector<Token> r;
    Lexer l(s);
    while (true) {
        Token t = l.next();
        if (t.kind == YYEOF) {
            break;
        } else if (t.kind == YYUNDEF) {
            printf("unrecognized charactor: %s\n", l.text());
            break;
        } else {
            r.push_back(std::move(t));
        }
    }
    return r;
}

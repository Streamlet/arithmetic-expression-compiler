#include "lex.h"
#include <memory>
#include <unordered_map>
#include <regex>

#ifdef WIN32
#define _USE_MATH_DEFINES
#endif
#include <math.h>

std::unordered_map<char, yytoken_kind_t> FIXED_TOKEN = {
    {'+', ADD}, {'-', SUB}, {'*', MUL}, {'/', DIV}, {'^', EXP}, {'%', MOD}, {'(', LPAREN}, {')', RPAREN}, {',', COMMA},
};

const char *PATTERN = "(^\\+)"
                      "|(^\\-)"
                      "|(^\\*)"
                      "|(^/)"
                      "|(^\\^)"
                      "|(^%)"
                      "|(^\\()"
                      "|(^\\))"
                      "|(^,)"
                      "|(^e)"
                      "|(^pi)"
                      "|(^(?:[0-9]+|[0-9]*\\.[0-9]+)(?:[Ee][+-]?[0-9]+)?)"
                      "|(^[A-Za-z_][A-Za-z_0-9]*)";

const std::regex REGEX(PATTERN);

enum PatternGroup {
    PG_ADD = 1,
    PG_SUB,
    PG_MUL,
    PG_DIV,
    PG_EXP,
    PG_MOD,
    PG_LPAREN,
    PG_RPAREN,
    PG_COMMA,
    PG_E,
    PG_PI,
    PG_NUM,
    PG_FUNC,
};

const yytoken_kind_t PATTERN_GROUP_TOKEN_MAP[] = {YYUNDEF, ADD,    SUB,   MUL, DIV, EXP, MOD,
                                                  LPAREN,  RPAREN, COMMA, NUM, NUM, NUM, FUNC};

struct Context
{
    char *yy_str;
    char *yytext;
    int yyleng;
    char yytext_end;
};

Lexer::Lexer(const char *yy_str) {
    Context *ctx = new Context{nullptr, nullptr, 0, '\0'};
    ctx->yy_str = new char[strlen(yy_str) + 1];
    strcpy(ctx->yy_str, yy_str);
    ctx->yytext = ctx->yy_str;
    ctx->yyleng = 0;
    ctx->yytext_end = '\0';
    yy_ctx = ctx;
}
Lexer::~Lexer() {
    delete[]((Context *)yy_ctx)->yy_str;
    delete (Context *)yy_ctx;
}

Token Lexer::next() {
    Context *ctx = (Context *)yy_ctx;
    if (!(ctx->yytext == ctx->yy_str && ctx->yyleng == 0)) {
        ctx->yytext[ctx->yyleng] = ctx->yytext_end;
        ctx->yytext += ctx->yyleng;
        ctx->yyleng = 1;
    }

    while (*ctx->yytext == ' ' || *ctx->yytext == '\t' || *ctx->yytext == '\n')
        ++ctx->yytext;

    if (*ctx->yytext == '\0') {
        return Token{YYEOF};
    }

    std::cmatch m;
    if (!std::regex_search(ctx->yytext, m, REGEX)) {
        return Token{YYUNDEF};
    }

    for (int t = PG_ADD; t <= PG_FUNC; ++t) {
        if (m[t].matched) {
            ctx->yyleng = m[t].second - m[t].first;
            ctx->yytext_end = ctx->yytext[ctx->yyleng];
            ctx->yytext[ctx->yyleng] = '\0';
            Token v{PATTERN_GROUP_TOKEN_MAP[t]};
            switch (t) {
            case PG_E:
                v.dval = M_E;
                break;
            case PG_PI:
                v.dval = M_PI;
                break;
            case PG_NUM:
                v.dval = atof(ctx->yytext);
                break;
            case PG_FUNC:
                v.str.assign(ctx->yytext, (size_t)ctx->yyleng);
                break;
            default:
                break;
            }
            return std::move(v);
        }
    }

    return Token{YYUNDEF};
}

const char *Lexer::text() {
    Context *ctx = (Context *)yy_ctx;
    return ctx->yytext;
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

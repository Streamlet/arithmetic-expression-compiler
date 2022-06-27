#include "lex.h"
#include <memory>
#include <unordered_map>

#ifdef WIN32
#define _USE_MATH_DEFINES
#endif
#include <math.h>

std::unordered_map<char, Token> FIXED_TOKEN = {
    {'+', Token::ADD}, {'-', Token::SUB},    {'*', Token::MUL},    {'/', Token::DIV},   {'^', Token::EXP},
    {'%', Token::MOD}, {'(', Token::LPAREN}, {')', Token::RPAREN}, {',', Token::COMMA},
};

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

TokenV Lexer::next() {
    Context *ctx = (Context *)yy_ctx;
    if (!(ctx->yytext == ctx->yy_str && ctx->yyleng == 0)) {
        ctx->yytext[ctx->yyleng] = ctx->yytext_end;
        ctx->yytext += ctx->yyleng;
        ctx->yyleng = 1;
    }

    while (*ctx->yytext == ' ' || *ctx->yytext == '\t' || *ctx->yytext == '\n')
        ++ctx->yytext;

    if (*ctx->yytext == '\0') {
        return TokenV{(Token)0};
    }

    const auto &it = FIXED_TOKEN.find(*ctx->yytext);
    if (it != FIXED_TOKEN.end()) {
        ctx->yytext_end = ctx->yytext[ctx->yyleng];
        ctx->yytext[ctx->yyleng] = '\0';
        return TokenV{it->second};
    }

    if (*ctx->yytext == 'e') {
        ctx->yytext_end = ctx->yytext[ctx->yyleng];
        ctx->yytext[ctx->yyleng] = '\0';
        return TokenV{NUM, M_E};
    }

    if (*ctx->yytext == 'p' && *(ctx->yytext + 1) == 'i') {
        ctx->yyleng = 2;
        ctx->yytext_end = ctx->yytext[ctx->yyleng];
        ctx->yytext[ctx->yyleng] = '\0';
        return TokenV{NUM, M_PI};
    }

    if ((*ctx->yytext >= '0' && *ctx->yytext <= '9') || *ctx->yytext == '.') {
        const char *p = ctx->yytext;
        bool has_dot = false, has_exp = false;
        while (*p != '\0') {
            if (*p == '.') {
                if (has_dot || has_exp)
                    break;
                if (p[1] == '\0' || !(p[1] >= '0' && p[1] <= '9'))
                    break;
                has_dot = true;
                p += 2;
            } else if (*p == 'E' || *p == 'e') {
                if (has_exp)
                    break;
                int digit_pos = p[1] == '+' || p[1] == '-' ? 2 : 1;
                if (p[digit_pos] == '\0' || !(p[digit_pos] >= '0' && p[digit_pos] <= '9'))
                    break;
                has_exp = true;
                p += digit_pos + 1;
            } else if (*p >= '0' && *p <= '9') {
                ++p;
            } else {
                break;
            }
        }
        if (p == ctx->yytext)
            return TokenV{(Token)-1};

        ctx->yyleng = p - ctx->yytext;
        ctx->yytext_end = ctx->yytext[ctx->yyleng];
        ctx->yytext[ctx->yyleng] = '\0';
        return TokenV{NUM, atof(ctx->yytext)};
    }

    if ((*ctx->yytext >= 'a' && *ctx->yytext <= 'z') || (*ctx->yytext >= 'A' && *ctx->yytext <= 'Z') ||
        *ctx->yytext == '_') {
        while ((ctx->yytext[ctx->yyleng] >= 'a' && ctx->yytext[ctx->yyleng] <= 'z') ||
               (ctx->yytext[ctx->yyleng] >= 'A' && ctx->yytext[ctx->yyleng] <= 'Z') || ctx->yytext[ctx->yyleng] == '_') {
            ++ctx->yyleng;
        }
        ctx->yytext_end = ctx->yytext[ctx->yyleng];
        ctx->yytext[ctx->yyleng] = '\0';
        return TokenV{FUNC, 0.0, ctx->yytext};
    }
    return TokenV{(Token)-1};
}

const char *Lexer::text() {
    Context *ctx = (Context *)yy_ctx;
    return ctx->yytext;
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

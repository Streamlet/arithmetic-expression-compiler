#include "lex.h"
#include <memory>
#include <unordered_map>

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

    if ((*ctx->yytext >= '0' && *ctx->yytext <= '9') || *ctx->yytext == '.') {
        while (ctx->yytext[ctx->yyleng] != '\0' && ctx->yytext[ctx->yyleng] >= '0' && ctx->yytext[ctx->yyleng] <= '9') {
            ++ctx->yyleng;
        }
        if (ctx->yytext[ctx->yyleng] == '.') {
            ++ctx->yyleng;
            while (ctx->yytext[ctx->yyleng] != '\0' && ctx->yytext[ctx->yyleng] >= '0' &&
                    ctx->yytext[ctx->yyleng] <= '9') {
                ++ctx->yyleng;
            }
        }
        if (ctx->yytext[ctx->yyleng] == 'E' || ctx->yytext[ctx->yyleng] == 'e') {
            ++ctx->yyleng;
            if (ctx->yytext[ctx->yyleng] == '+' || ctx->yytext[ctx->yyleng] == '-') {
                ++ctx->yyleng;
            }
            while (ctx->yytext[ctx->yyleng] != '\0' && ctx->yytext[ctx->yyleng] >= '0' &&
                   ctx->yytext[ctx->yyleng] <= '9') {
                ++ctx->yyleng;
            }
        }
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
        return TokenV{NUM, 0.0, ctx->yytext};
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

void print(std::vector<TokenV> token_list) {
    for (const auto &t : token_list) {
        switch (t.token) {
        case NUM:
            printf("%s %lf\n", TOKEN_NAMES[t.token], t.dval);
            break;
        case FUNC:
            printf("%s %s\n", TOKEN_NAMES[t.token], t.str.c_str());
            break;
        default:
            printf("%s\n", TOKEN_NAMES[t.token]);
            break;
        }
    }
}

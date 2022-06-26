#include "lex.h"
#include <memory>
#include <unordered_map>
#include <regex>

std::unordered_map<char, Token> FIXED_TOKEN = {
    {'+', Token::ADD}, {'-', Token::SUB},    {'*', Token::MUL},    {'/', Token::DIV},   {'^', Token::EXP},
    {'%', Token::MOD}, {'(', Token::LPAREN}, {')', Token::RPAREN}, {',', Token::COMMA},
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
                      "|(^(?:[0-9]+|[0-9]*\\.[0-9]+)(?:[Ee][+-]?[0-9]+)?)"
                      "|(^[A-Za-z_][A-Za-z_0-9]*)";

const std::regex REGEX(PATTERN);

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

    std::cmatch m;
    if (!std::regex_search(ctx->yytext, m, REGEX)) {
        return TokenV{(Token)-1};
    }

    for (int t = ADD; t <= FUNC; ++t) {
        if (m[t].matched) {
            ctx->yyleng = m[t].second - m[t].first;
            ctx->yytext_end = ctx->yytext[ctx->yyleng];
            ctx->yytext[ctx->yyleng] = '\0';
            TokenV v{(Token)t};
            switch (t) {
            case NUM:
                v.dval = atof(ctx->yytext);
                break;
            case FUNC:
                v.str.assign(ctx->yytext, (size_t)ctx->yyleng);
                break;
            default:
                break;
            }
            return std::move(v);
        }
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

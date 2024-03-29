%{
#ifdef USE_LEX_WITH_YACC
#include "../../parser/bison/parser.tab.hh"
#else
#include "../token_type.h"
union {
    double dval;
    struct {
        const char *str;
        int len;
    } sval;
} yylval;
#endif

#ifdef _WIN32
#define YYEOF   0
#define YYUNDEF 257
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <stdlib.h>


%}

number      ([0-9]+|[0-9]*\.[0-9]+)([Ee][+-]?[0-9]+)?
id          [A-Za-z_][A-Za-z_0-9]*

%%

" "         ;
\t          ;
\n          { return YYEOF; }

"+"         { return ADD; }
"-"         { return SUB; }
"*"         { return MUL; }
"/"         { return DIV; }
"^"         { return EXP; }
"%"         { return MOD; }
"("         { return LPAREN; }
")"         { return RPAREN; }
","         { return COMMA; }
"e"         { yylval.dval = M_E; return NUM; }
"pi"        { yylval.dval = M_PI; return NUM; }
{number}    { yylval.dval = atof(yytext); return NUM; }
{id}        { yylval.sval.str = yytext; yylval.sval.len = yyleng; return FUNC; }

.           { return YYUNDEF; }

%%

double yy_get_dval() {
    return yylval.dval;
}

int yy_get_strlen() {
    return yylval.sval.len;
}

const char *yy_get_str() {
    return yylval.sval.str;
}

const char *yy_get_current_text() {
    return yytext;
}

struct LexContext {
    YY_BUFFER_STATE yy_buffer_state;
};

void *yy_init_ctx(const char *yy_str) {
    struct LexContext *ctx = (struct LexContext *)malloc(sizeof(struct LexContext));
    ctx->yy_buffer_state = yy_scan_string(yy_str);
    return ctx;
}

void yy_free_ctx(void *yy_ctx) {
    struct LexContext *ctx = (struct LexContext *)yy_ctx;
    yy_delete_buffer(ctx->yy_buffer_state);
    free(ctx);
}

int yywrap() {
    return 1;
}

%{

#include "../token.h"

#ifdef WIN32
#define _USE_MATH_DEFINES
#endif
#include <math.h>

union {
    double dval;
    int strlen;
    const char *str;
} yylval;

%}

number      ([0-9]+|[0-9]*\.[0-9]+)([Ee][+-]?[0-9]+)?
id          [A-Za-z_][A-Za-z_0-9]*

%%

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
{id}        { yylval.strlen = yyleng; yylval.str = yytext; return FUNC; }
.           { return -1; }

%%

double yy_get_dval() {
    return yylval.dval;
}

int yy_get_strlen() {
    return yylval.strlen;
}

const char *yy_get_str() {
    return yylval.str;
}

const char *yy_get_current_text() {
    return yylval.str;
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

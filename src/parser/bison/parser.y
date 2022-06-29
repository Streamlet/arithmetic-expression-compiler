%{
#include <stdio.h>

#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <string.h>

extern int yylex();
int yyerror(char* s);

double yy_result = 0.0f;
const char *yy_error_type = NULL;
const char *yy_error_text = NULL;
%}
 
%token ADD
%token SUB
%token MUL
%token DIV
%token EXP
%token MOD
%token LPAREN
%token RPAREN
%token COMMA
%token NUM
%token FUNC

%left ADD SUB
%left MUL DIV MOD
%right EXP

%union {
    double dval;
    struct {
        const char *str;
        int len;
    } sval;
}
%type <dval> line expr terms term factors factor expees expee NUM
%type <sval> FUNC

%%
line: expr { $$ = $1; yy_result = $$; }
expr: terms { $$ = $1; }
terms: term { $$ = $1; }
     | ADD term { $$ = $2; }
     | SUB term { $$ = -$2; }
     | terms ADD term { $$ = $1 + $3; }
     | terms SUB term { $$ = $1 - $3; }
     ;
term: factors { $$ = $1; }
factors: factor { $$ = $1; }
       | factors MUL factor { $$ = $1 * $3; }
       | factors DIV factor { $$ = $1 / $3; }
       | factors MOD factor { $$ = fmod($1, $3); }
    ;
factor: expees { $$ = $1; }
expees: expee { $$ = $1; }
      | expee EXP expees  { $$ = pow($1, $3); }
      ;
expee: NUM  { $$ = $1; }
     | LPAREN expr RPAREN  { $$ = $2; }
     | FUNC LPAREN expr RPAREN {
        if (strncmp("ln", $1.str, $1.len) == 0) {
            $$ = log($3);
        } else if (strncmp("lg", $1.str, $1.len) == 0) {
            $$ = log10($3);
        } else if (strncmp("sqrt", $1.str, $1.len) == 0) {
            $$ = sqrt($3);
        }
      }
     | FUNC LPAREN expr COMMA expr RPAREN { 
        if (strncmp("log", $1.str, $1.len) == 0) {
            $$ = log($5)/log($3);
        }
      }
     ;
%%

extern void *yy_init_ctx(const char *yy_str);
extern void yy_free_ctx(void *yy_ctx);
extern char *yy_get_current_text();

int yyerror(char* s) {
    yy_error_type = s;
    yy_error_text = yy_get_current_text();
    return 0;
}

double yy_get_result() {
    return yy_result;
}

const char *yy_get_error_type() {
    return yy_error_type;
}

const char *yy_get_error_text() {
    return yy_error_text;
}

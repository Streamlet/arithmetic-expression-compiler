%{
#include <stdio.h>

#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <string.h>

extern int yylex();
int yyerror(char* s);
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
    } str;
}
%type <dval> line expr terms term factors factor expees expee NUM
%type <str> FUNC

%%
line: expr { $$ = $1; printf("%lf\n", $$); }
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
 
int main() {
    while (1) {
        yyparse();
    }
    return 0;
}
 
int yyerror(char* s) {
    printf("Error: %s\n", s);
    return 0;
}

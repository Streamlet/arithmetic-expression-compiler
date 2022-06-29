#include "parser.h"

extern "C" void *yy_init_ctx(const char *yy_str);
extern "C" void yy_free_ctx(void *yy_ctx);
extern "C" int yyparse();
extern "C" double yy_get_result();
extern "C" const char *yy_get_error_type();
extern "C" const char *yy_get_error_text();

Parser::Parser(const char *yy_str) {
    yy_ctx = yy_init_ctx(yy_str);
}

Parser::~Parser() {
    yy_free_ctx(yy_ctx);
}

bool Parser::parse() {
    return yyparse() == 0;
}

double Parser::result() {
    return yy_get_result();
}

const char *Parser::error_type() {
    return yy_get_error_type();
}

const char *Parser::error_text() {
    return yy_get_error_text();
}

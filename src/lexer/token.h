#pragma once

#include "token_type.h"
#include <string>
#include <vector>

struct Token {
    yytoken_kind_t kind;
    double dval;
    std::string sval;
};

const char *token_name(yytoken_kind_t kind);
void print(std::vector<Token> token_list);

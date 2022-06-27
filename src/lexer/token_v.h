#pragma once

#include "token.h"
#include <string>
#include <vector>

struct TokenV {
    Token token;
    double dval;
    std::string str;
};

void print(std::vector<TokenV> token_list);

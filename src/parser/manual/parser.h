#pragma once

#include "../ast.h"
#include <memory>


class Parser {
public:
    Parser(const char *yy_str);
    ~Parser();

    bool parse();
    std::unique_ptr<ASTNode> result();
    const std::string &error();

private:
    ASTNode *line();
    ASTNode *expr();
    ASTNode *term();
    ASTNode *factor();
    ASTNode *expee();
    ASTFunction *params();
    ASTNumber *num();

private:
    struct ParseContext;
    ParseContext *yy_ctx;
};

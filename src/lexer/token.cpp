#include "token.h"

const char *TOKEN_NAMES[] = {
    "UNDEF", "ADD", "SUB", "MUL", "DIV", "EXP", "MOD", "LPAREN", "RPAREN", "COMMA", "NUM", "FUNC",
};

const char *token_name(yytoken_kind_t kind) {
    if (kind >= YYUNDEF)
        return TOKEN_NAMES[kind - YYUNDEF];
    else if (kind == YYEOF)
        return "EOF";
    else
        assert(false);
    return "";
}

void print(std::vector<Token> token_list) {
    for (const auto &t : token_list) {
        switch (t.kind) {
        case NUM:
            printf("%s %lf\n", token_name(t.kind), t.dval);
            break;
        case FUNC:
            printf("%s %s\n", token_name(t.kind), t.sval.c_str());
            break;
        default:
            printf("%s\n", token_name(t.kind));
            break;
        }
    }
}

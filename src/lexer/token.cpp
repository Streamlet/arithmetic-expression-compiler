#include "token_v.h"

const char *TOKEN_NAMES[] = {
    "EOF", "ADD", "SUB", "MUL", "DIV", "EXP", "MOD", "LPAREN", "RPAREN", "COMMA", "NUM", "FUNC",
};

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

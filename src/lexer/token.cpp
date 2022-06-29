#include "token.h"

const char *TOKEN_NAMES[] = {
    "UNDEF", "ADD", "SUB", "MUL", "DIV", "EXP", "MOD", "LPAREN", "RPAREN", "COMMA", "NUM", "FUNC",
};

void print(std::vector<Token> token_list) {
    for (const auto &t : token_list) {
        switch (t.kind) {
        case NUM:
            printf("%s %lf\n", TOKEN_NAMES[t.kind - YYUNDEF], t.dval);
            break;
        case FUNC:
            printf("%s %s\n", TOKEN_NAMES[t.kind - YYUNDEF], t.sval.c_str());
            break;
        default:
            printf("%s\n", TOKEN_NAMES[t.kind - YYUNDEF]);
            break;
        }
    }
}

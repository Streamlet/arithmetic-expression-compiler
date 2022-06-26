#include "lex.h"
#include <stdio.h>

int main() {
    char buffer[100] = {};
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        auto token_list = lex(buffer);
        print(token_list);
    }
    return 0;
}

#include "parser.h"
#include <stdio.h>

int main() {
    char buffer[100] = {};
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        Parser p(buffer);
        if (p.parse())
            printf("%lf\n", p.result());
        else
            printf("%s: %s\n", p.error_type(), p.error_text());
    }
    return 0;
}

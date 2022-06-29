#pragma once

class Parser {
public:
    Parser(const char *yy_str);
    ~Parser();

    bool parse();
    double result();
    const char *error_type();
    const char *error_text();

private:
    void *yy_ctx;
};

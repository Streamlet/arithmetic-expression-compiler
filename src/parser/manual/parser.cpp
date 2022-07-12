#include "parser.h"
#include "../../lexer/manual/lex.h"
#include <vector>
#include <sstream>

class parse_exception : public std::exception {
public:
    parse_exception(std::string error) : error(std::move(error)) {
    }
    const char *what() const _NOEXCEPT override {
        return error.c_str();
    }

private:
    std::string error;
};

struct Parser::ParseContext {
    ParseContext(const char *yy_str) : lexer(yy_str) {
    }

    Lexer lexer;
    std::vector<Token> cached_tokens;

    const Token &next_token() {
        if (cached_tokens.empty())
            cached_tokens.push_back(std::move(lexer.next()));
        return cached_tokens[0];
    }

    void use_token(yytoken_kind_t kind) {
        if (cached_tokens.empty()) {
            Token t = lexer.next();
            assert(t.kind == kind);
            if (t.kind != kind) {
                std::stringstream ss;
                ss << "syntax error: except " << token_name(kind) << ", " << token_name(t.kind) << " given";
                throw parse_exception(ss.str());
            }
        } else {
            assert(cached_tokens.size() == 1 && cached_tokens[0].kind == kind);
            if (cached_tokens[0].kind != kind) {
                std::stringstream ss;
                ss << "syntax error: except " << token_name(kind) << ", " << token_name(cached_tokens[0].kind) << " given";
                throw parse_exception(ss.str());
            }

            cached_tokens.pop_back();
        }
    }

    std::unique_ptr<ASTNode> result;
    std::string error;
};

Parser::Parser(const char *yy_str) {
    yy_ctx = new ParseContext(yy_str);
}

Parser::~Parser() {
    delete yy_ctx;
}

bool Parser::parse() {
    try {
        yy_ctx->result = std::unique_ptr<ASTNode>(line());
        return true;
    } catch (parse_exception e) {
        yy_ctx->error = e.what();
        return false;
    }
}

//
// LR 文法改为 LL 文法：
//
// line: expr;
// expr: (ADD|SUB)? term | expr (ADD|SUB) term;
// => expr: (ADD|SUB)? term ( (ADD|SUB) term )*;
// term: factor | term (MUL|DIV|MOD) factor;
// => term: factor ( (MUL|DIV|MOD) factor )*;
// factor: expee (EXP factor)?;
// expee: num | LPAREN expr RPAREN | FUNC LPAREN params RPAREN;
// params: expr | params COMMA expr;
// => params: expr (COMMA expr)*;
// num: NUM;
//

// line: expr;
ASTNode *Parser::line() {
    ASTNode *line = expr();
    yy_ctx->use_token(YYEOF);
    return line;
}

// expr: (ADD|SUB)? term ( (ADD|SUB) term )*;
ASTNode *Parser::expr() {
    const Token &t = yy_ctx->next_token();
    ASTNode *expr_result = nullptr;
    switch (t.kind) {
    case ADD:
        yy_ctx->use_token(ADD);
        expr_result = new ASTUnaryOperator(ASTUnaryOperator::POS, term());
        break;
    case SUB:
        yy_ctx->use_token(SUB);
        expr_result = new ASTUnaryOperator(ASTUnaryOperator::NEG, term());
        break;
    default:
        expr_result = term();
    }
    bool loop = true;
    while (loop) {
        const Token &tt = yy_ctx->next_token();
        switch (tt.kind) {
        case ADD:
            yy_ctx->use_token(ADD);
            expr_result = new ASTBinaryOperator(ASTBinaryOperator::ADD, expr_result, term());
            break;
        case SUB:
            yy_ctx->use_token(SUB);
            expr_result = new ASTBinaryOperator(ASTBinaryOperator::SUB, expr_result, term());
            break;
        default:
            loop = false;
            break;
        }
    }

    return expr_result;
}

// term: factor ( (MUL|DIV|MOD) factor )*;
ASTNode *Parser::term() {
    ASTNode *term_result = factor();
    bool loop = true;
    while (loop) {
        const Token &t = yy_ctx->next_token();
        switch (t.kind) {
        case MUL:
            yy_ctx->use_token(MUL);
            term_result = new ASTBinaryOperator(ASTBinaryOperator::MUL, term_result, factor());
            break;
        case DIV:
            yy_ctx->use_token(DIV);
            term_result = new ASTBinaryOperator(ASTBinaryOperator::DIV, term_result, factor());
            break;
        case MOD:
            yy_ctx->use_token(MOD);
            term_result = new ASTBinaryOperator(ASTBinaryOperator::MOD, term_result, factor());
            break;
        default:
            loop = false;
            break;
        }
    }
    return term_result;
}

// factor: expee (EXP factor)?;
ASTNode *Parser::factor() {
    ASTNode *factor_result = expee();
    const Token &t = yy_ctx->next_token();
    if (t.kind == EXP) {
        yy_ctx->use_token(EXP);
        factor_result = new ASTBinaryOperator(ASTBinaryOperator::EXP, factor_result, factor());
    }
    return factor_result;
}

// expee: num | LPAREN expr RPAREN | FUNC LPAREN params RPAREN;
ASTNode *Parser::expee() {
    const Token &t = yy_ctx->next_token();
    ASTNode *expee_result = nullptr;
    switch (t.kind) {
    case NUM:
        expee_result = num();
        break;
    case LPAREN:
        yy_ctx->use_token(LPAREN);
        expee_result = expr();
        yy_ctx->use_token(RPAREN);
        break;
    case FUNC: {
        std::string func_name = t.sval;
        yy_ctx->use_token(FUNC);
        yy_ctx->use_token(LPAREN);
        expee_result = params();
        yy_ctx->use_token(RPAREN);
        std::string err = ((ASTFunction *)expee_result)->assign_name(func_name.c_str(), func_name.length());
        if (!err.empty())
            throw parse_exception(std::move(err));
    } break;
    default:
        std::stringstream ss;
        ss << "syntax error: except " << token_name(NUM) << ", " << token_name(LPAREN) << ", or " << token_name(FUNC)
           << ", " << token_name(t.kind) << " given";
        throw parse_exception(ss.str());
        break;
    }
    return expee_result;
}

// params: expr (COMMA expr)*;
ASTFunction *Parser::params() {
    ASTFunction *params_result = new ASTFunction;
    params_result->add_argument(expr());
    while (true) {
        const Token &t = yy_ctx->next_token();
        if (t.kind == COMMA) {
            yy_ctx->use_token(COMMA);
            params_result->add_argument(expr());
        } else {
            break;
        }
    }
    return params_result;
}

// num: NUM;
ASTNumber *Parser::num() {
    const Token &t = yy_ctx->next_token();
    double num = t.dval;
    yy_ctx->use_token(NUM);
    ASTNumber *num_result = new ASTNumber(t.dval);
    return num_result;
}

std::unique_ptr<ASTNode> Parser::result() {
    return std::move(yy_ctx->result);
}

const std::string &Parser::error() {
    return yy_ctx->error;
}

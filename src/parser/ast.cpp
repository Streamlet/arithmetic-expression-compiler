#include "ast.h"
#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <sstream>
#include <assert.h>

ASTNode::ASTNode(ASTNodeType type) : type(type) {
}

ASTNode::~ASTNode() {
}

ASTNumber::ASTNumber(double dval) : ASTNode(NUMBER), dval(dval) {
}

double ASTNumber::value() {
    return dval;
}

ASTUnaryOperator::ASTUnaryOperator(Operator op, ASTNode *operand) : ASTNode(UNARY_OPERATOR), op(op), operand(operand) {
}

double ASTUnaryOperator::value() {
    switch (op) {
    case POS:
        return operand->value();
    case NEG:
        return -operand->value();
    }
    assert(false);
    return 0.0;
}

ASTBinaryOperator::ASTBinaryOperator(Operator op, ASTNode *left, ASTNode *right)
    : ASTNode(BINARY_OPERATOR), op(op), left(left), right(right) {
}

double ASTBinaryOperator::value() {
    switch (op) {
    case ADD:
        return left->value() + right->value();
    case SUB:
        return left->value() - right->value();
    case MUL:
        return left->value() * right->value();
    case DIV:
        return left->value() / right->value();
    case EXP:
        return pow(left->value(), right->value());
    case MOD:
        return fmod(left->value(), right->value());
    }
    assert(false);
    return 0.0;
}

struct FuncDef {
    ASTFunction::Function func;
    const char *name;
    int args;
};

FuncDef FUNC_DEF[] = {
    {ASTFunction::SQRT, "sqrt", 1},
    {ASTFunction::LOG, "log", 2},
    {ASTFunction::LN, "ln", 1},
    {ASTFunction::LG, "lg", 1},
    {ASTFunction::SIN, "sin", 1},
    {ASTFunction::COS, "cos", 1},
    {ASTFunction::TAN, "tan", 1},
    {ASTFunction::TAN, "tg", 1},
    {ASTFunction::COT, "cot", 1},
    {ASTFunction::COT, "ctg", 1},
};

ASTFunction::ASTFunction() : ASTNode(FUNCTION) {
}

double ASTFunction::value() {
    switch (func) {
    case SQRT:
        return sqrt(children[0]->value());
    case LOG:
        return log(children[1]->value()) / log(children[0]->value());
    case LN:
        return log(children[0]->value());
    case LG:
        return log10(children[0]->value());
    case SIN:
        return sin(children[0]->value());
    case COS:
        return cos(children[0]->value());
    case TAN:
        return tan(children[0]->value());
    case COT:
        return 1 / tan(children[0]->value());
    }
    assert(false);
    return 0.0;
}

void ASTFunction::add_child(ASTNode *child) {
    children.push_back(std::unique_ptr<ASTNode>(child));
}

bool ASTFunction::assign_name(const char *name, int len, int(yyerror)(const char *s)) {
    for (int i = 0; i < sizeof(FUNC_DEF) / sizeof(FuncDef); ++i) {
        if (strncmp(FUNC_DEF[i].name, name, len) == 0) {
            func = FUNC_DEF[i].func;
            if (children.size() != FUNC_DEF[i].args) {
                std::stringstream ss;
                ss << "function '" << FUNC_DEF[i].name << "' needs " << FUNC_DEF[i].args << " argument(s), "
                   << children.size() << " provided.";
                yyerror(ss.str().c_str());
                return false;
            }
            return true;
        }
    }
    std::stringstream ss;
    ss << "function '" << std::string(name, len) << "' not found.";
    yyerror(ss.str().c_str());
    return false;
}

void print_result(ASTNode *node) {
    printf("%lf\n", node->value());
}

const char *UNARY_OPERATOR_NAMES[] = {"POS", "NEG"};
const char *BINARY_OPERATOR_NAMES[] = {"ADD", "SUB", "MUL", "DIV", "EXP", "MOD"};
const char *FUNC_NAMES[] = {"sqrt", "log", "ln", "lg", "sin", "cos", "tan", "cot"};

void print_struct_internal(std::string indent, ASTNode *node) {
    switch (node->type) {
    case ASTNode::NUMBER:
        printf("%s%lg\n", indent.c_str(), ((ASTNumber *)node)->dval);
        break;
    case ASTNode::UNARY_OPERATOR:
        printf("%s%s = %lg\n", indent.c_str(), UNARY_OPERATOR_NAMES[((ASTUnaryOperator *)node)->op], node->value());
        print_struct_internal(indent + "  ", ((ASTUnaryOperator *)node)->operand.get());
        break;
    case ASTNode::BINARY_OPERATOR:
        printf("%s%s = %lg\n", indent.c_str(), BINARY_OPERATOR_NAMES[((ASTBinaryOperator *)node)->op], node->value());
        print_struct_internal(indent + "  ", ((ASTBinaryOperator *)node)->left.get());
        print_struct_internal(indent + "  ", ((ASTBinaryOperator *)node)->right.get());
        break;
    case ASTNode::FUNCTION:
        printf("%s%s = %lg\n", indent.c_str(), FUNC_NAMES[((ASTFunction *)node)->func], node->value());
        for (const auto &child : ((ASTFunction *)node)->children) {
            print_struct_internal(indent + "  ", child.get());
        }
        break;
    default:
        assert(false);
    }
}

void print_struct(ASTNode *node) {
    print_struct_internal("", node);
}
int print_graphviz_internal(int count, ASTNode *node) {
    int next = count + 1;
    switch (node->type) {
    case ASTNode::NUMBER:
        printf("  n%d [ label=\"%lg\" shape=circle ];\n", count, ((ASTNumber *)node)->dval);
        break;
    case ASTNode::UNARY_OPERATOR:
        printf("  n%d [ label=\"%s\\n(=%lg)\" shape=doublecircle ];\n", count,
               UNARY_OPERATOR_NAMES[((ASTUnaryOperator *)node)->op], node->value());
        printf("  n%d->n%d\n", count, next);
        next = print_graphviz_internal(next, ((ASTUnaryOperator *)node)->operand.get());
        break;
    case ASTNode::BINARY_OPERATOR:
        printf("  n%d [ label=\"%s\\n(=%lg)\" shape=doublecircle ];\n", count,
               BINARY_OPERATOR_NAMES[((ASTBinaryOperator *)node)->op], node->value());
        printf("  n%d->n%d;\n", count, next);
        next = print_graphviz_internal(next, ((ASTBinaryOperator *)node)->left.get());
        printf("  n%d->n%d;\n", count, next);
        next = print_graphviz_internal(next, ((ASTBinaryOperator *)node)->right.get());
        break;
    case ASTNode::FUNCTION:
        printf("  n%d [label=\"%s\\n(=%lg)\" shape=rect ];\n", count, FUNC_NAMES[((ASTFunction *)node)->func],
               node->value());
        for (const auto &child : ((ASTFunction *)node)->children) {
            printf("  n%d->n%d;\n", count, next);
            next = print_graphviz_internal(next, child.get());
        }
        break;
    default:
        assert(false);
    }
    return next;
}
void print_graphviz(ASTNode *node) {
    printf("digraph {\n"
           "  rankdir=TD;\n");
    print_graphviz_internal(0, node);
    printf("}\n");
}

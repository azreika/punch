#pragma once

#include "AstFunction.h"
#include "AstNode.h"
#include "AstProgram.h"
#include "AstStatement.h"

#include <cassert>

template <class T, typename... Args> class AstVisitor {
public:
    AstVisitor() = default;

    T visit(const AstNode* node, Args... args) {

#define LEAF(Kind)                                                             \
    if (const auto* n = dynamic_cast<const Ast##Kind*>(node))                  \
        return visit##Kind(n, args...);

        LEAF(Program);
        LEAF(FunctionDecl);
        LEAF(Variable);
        LEAF(Assignment);
        LEAF(BinaryExpression);
        LEAF(NumberLiteral);
        LEAF(StringLiteral);
        LEAF(FunctionCall);
        LEAF(RawBashExpression);
        LEAF(RawPunchExpression);
        LEAF(RawEnvironment);
        LEAF(Return);
        LEAF(SimpleConditional);
        LEAF(BranchingConditional);
        LEAF(True);
        LEAF(False);
        LEAF(StatementBlock);

#undef LEAF

        std::cerr << "Unsupported type: " << typeid(node).name() << std::endl;
        assert(false && "missing visitor type");
    }

protected:
    virtual T visitNode(const AstNode* n) { return T(); }

#define CHILD(Node, Parent)                                                    \
    virtual T visit##Node(const Ast##Node* n, Args... args) {                  \
        return visit##Parent(n, args...);                                      \
    }

    CHILD(Program, Node);
    CHILD(FunctionDecl, Node);
    CHILD(Statement, Node);
    CHILD(Expression, Statement);
    CHILD(Variable, Expression);
    CHILD(FunctionCall, Expression);
    CHILD(Assignment, Statement);
    CHILD(BinaryExpression, Expression);
    CHILD(Literal, Expression);
    CHILD(NumberLiteral, Literal);
    CHILD(StringLiteral, Literal);
    CHILD(RawExpression, Expression);
    CHILD(RawBashExpression, RawExpression);
    CHILD(RawPunchExpression, RawExpression);
    CHILD(RawEnvironment, Expression);
    CHILD(Return, Statement);
    CHILD(Conditional, Statement);
    CHILD(SimpleConditional, Conditional);
    CHILD(BranchingConditional, Conditional);
    CHILD(Condition, Node);
    CHILD(True, Condition);
    CHILD(False, Condition);
    CHILD(StatementBlock, Statement);

#undef CHILD
};

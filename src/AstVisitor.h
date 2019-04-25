#pragma once

#include "AstFunction.h"
#include "AstNode.h"
#include "AstProgram.h"
#include "AstStatement.h"

class AstVisitor {
public:
    AstVisitor() = default;

    void visit(const AstNode* node) {

#define LEAF(Kind)                                                          \
    if (const auto* n = dynamic_cast<const Ast##Kind*>(node))                 \
        return visit##Kind(n);

        LEAF(Program);
        LEAF(Function);
        LEAF(Variable);
        LEAF(Assignment);
        LEAF(BinaryExpression);
        LEAF(NumberLiteral);
        LEAF(StringLiteral);
        LEAF(RawBashExpression);
        LEAF(RawPunchExpression);
        LEAF(RawEnvironment);
#undef LEAF

        std::cerr << "Unsupported type: " << typeid(node).name() << std::endl;
        assert(false && "missing visitor type");
    }

protected:
    virtual void visitNode(const AstNode* n) { return; }

#define CHILD(Node, Parent)                                                     \
    virtual void visit##Node(const Ast##Node* n) { return visit##Parent(n); }

    CHILD(Program, Node);
    CHILD(Function, Node);
    CHILD(Statement, Node);
    CHILD(Expression, Statement);
    CHILD(Variable, Expression);
    CHILD(Assignment, Statement);
    CHILD(BinaryExpression, Expression);
    CHILD(Literal, Expression);
    CHILD(NumberLiteral, Literal);
    CHILD(StringLiteral, Literal);
    CHILD(RawExpression, Expression);
    CHILD(RawBashExpression, RawExpression);
    CHILD(RawPunchExpression, RawExpression);
    CHILD(RawEnvironment, Expression);

#undef CHILD
};

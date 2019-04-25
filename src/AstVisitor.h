#include "AstNode.h"
#include "AstStatement.h"
#include "AstFunction.h"
#include "AstProgram.h"

template <class T> class AstVisitor {
public:
    AstVisitor() = default;

    virtual T visitNode() { return T(); }

#define CHILD(Node, Parent)                                                     \
    virtual T visit##Node(const Ast##Node* n) { return visit##Parent(n); }

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
};

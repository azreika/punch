#pragma once

#include "AstNode.h"
#include "Token.h"

#include <iostream>
#include <memory>
#include <vector>

class AstStatement : public AstNode {};

class AstExpression : public AstStatement {};

class AstVariable : public AstExpression {
public:
    AstVariable(std::string ident) : ident(ident) {}

    void print(std::ostream& os) const override { os << ident; }

private:
    std::string ident;
};

class AstAssignment : public AstStatement {
public:
    AstAssignment(bool declaration, std::unique_ptr<AstVariable> var,
                  std::unique_ptr<AstExpression> expr)
        : declaration(declaration), var(std::move(var)), expr(std::move(expr)) {
    }

    void print(std::ostream& os) const override {
        if (declaration) {
            os << "var ";
        }
        var->print(os);
        os << " = ";
        expr->print(os);
    }

private:
    bool declaration;
    std::unique_ptr<AstVariable> var;
    std::unique_ptr<AstExpression> expr;
};

class AstBinaryExpression : public AstExpression {
public:
    AstBinaryExpression(TokenType op, std::unique_ptr<AstExpression> lhs,
                        std::unique_ptr<AstExpression> rhs)
        : op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}

    void print(std::ostream& os) const override {
        os << getSymbolForTokenType(op);
        os << "(" << *lhs << ", " << *rhs << ")";
    }

private:
    TokenType op;
    std::unique_ptr<AstExpression> lhs;
    std::unique_ptr<AstExpression> rhs;
};

class AstLiteral : public AstExpression {};

class AstNumberLiteral : public AstLiteral {
public:
    AstNumberLiteral(int number) : number(number) {}

    void print(std::ostream& os) const override { os << number; }

private:
    int number;
};

class AstStringLiteral : public AstLiteral {
public:
    AstStringLiteral(std::string string) : string(string) {}

private:
    std::string string;
};

class AstRawExpression : public AstExpression {};

class AstRawBashExpression : public AstRawExpression {
public:
    AstRawBashExpression(std::string expr) : expr(expr) {}

    void print(std::ostream& os) const override { os << expr; }

private:
    std::string expr;
};

class AstRawPunchExpression : public AstRawExpression {
public:
    AstRawPunchExpression(std::unique_ptr<AstExpression> expr)
        : expr(std::move(expr)) {}

    void print(std::ostream& os) const override {
        os << "$[";
        expr->print(os);
        os << "]";
    }

private:
    std::unique_ptr<AstExpression> expr;
};

class AstRawEnvironment : public AstExpression {
public:
    AstRawEnvironment() : expressions({}) {}

    AstRawEnvironment(
        std::vector<std::unique_ptr<AstRawExpression>> expressions)
        : expressions(std::move(expressions)) {}

    void addRawExpression(std::unique_ptr<AstRawExpression> expr) {
        expressions.push_back(std::move(expr));
    }

    void print(std::ostream& os) const override {
        os << "raw {" << std::endl;
        for (const auto& expr : expressions) {
            expr->print(os);
        }
        os << "}";
    }

private:
    std::vector<std::unique_ptr<AstRawExpression>> expressions;
};
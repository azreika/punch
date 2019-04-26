#pragma once

#include "AstNode.h"
#include "Tools.h"

#include <iostream>
#include <memory>
#include <vector>

class AstStatement : public AstNode {};

class AstExpression : public AstStatement {};

class AstVariable : public AstExpression {
public:
    AstVariable(std::string ident) : ident(ident) {}

    // TODO: change this
    std::string getName() const { return ident; }

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

    AstVariable* getVariable() const { return var.get(); }

    AstExpression* getExpression() const { return expr.get(); }

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

class AstFunctionCall : public AstExpression {
public:
    AstFunctionCall(std::string name,
                    std::vector<std::unique_ptr<AstExpression>> arguments)
        : name(name), arguments(std::move(arguments)) {}

    std::string getName() const {
        return name;
    }

    std::vector<AstExpression*> getArguments() const {
        return Tools::toPtrVector(arguments);
    }

private:
    std::string name;
    std::vector<std::unique_ptr<AstExpression>> arguments;
};

class AstBinaryExpression : public AstExpression {
public:
    AstBinaryExpression(char op, std::unique_ptr<AstExpression> lhs,
                        std::unique_ptr<AstExpression> rhs)
        : op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}

    void print(std::ostream& os) const override {
        os << op;
        os << "(" << *lhs << ", " << *rhs << ")";
    }

    char getOperator() const { return op; }

    AstExpression* getLHS() const { return lhs.get(); }

    AstExpression* getRHS() const { return rhs.get(); }

private:
    char op; // TODO: use enum
    std::unique_ptr<AstExpression> lhs;
    std::unique_ptr<AstExpression> rhs;
};

class AstLiteral : public AstExpression {};

class AstNumberLiteral : public AstLiteral {
public:
    AstNumberLiteral(int number) : number(number) {}

    int getNumber() const { return number; }

    void print(std::ostream& os) const override { os << number; }

private:
    int number;
};

class AstStringLiteral : public AstLiteral {
public:
    AstStringLiteral(std::string string) : string(string) {}

    std::string getString() const { return string; }

    void print(std::ostream& os) const override { os << string; }

private:
    std::string string;
};

class AstRawExpression : public AstExpression {};

class AstRawBashExpression : public AstRawExpression {
public:
    AstRawBashExpression(std::string expr) : expr(expr) {}

    std::string getExpression() const { return expr; }

    void print(std::ostream& os) const override { os << expr; }

private:
    std::string expr;
};

class AstRawPunchExpression : public AstRawExpression {
public:
    AstRawPunchExpression(std::unique_ptr<AstExpression> expr)
        : expr(std::move(expr)) {}

    AstExpression* getExpression() const { return expr.get(); }

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

    std::vector<AstRawExpression*> getExpressions() const {
        return Tools::toPtrVector(expressions);
    }

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

class AstReturn : public AstStatement {
public:
    AstReturn(std::unique_ptr<AstExpression> expr) : expr(std::move(expr)) {}

    AstExpression* getExpression() const { return expr.get(); }

    void print(std::ostream& os) const override {
        os << "return ";
        expr->print(os);
    }

private:
    std::unique_ptr<AstExpression> expr;
};

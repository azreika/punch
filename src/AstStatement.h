#pragma once

#include "AstNode.h"
#include "Tools.h"

#include <iostream>
#include <memory>
#include <vector>

class AstStatement : public AstNode {};

class AstStatementBlock : public AstStatement {
public:
    AstStatementBlock() = default;

    void appendStatement(std::unique_ptr<AstStatement> stmt) {
        stmts.push_back(std::move(stmt));
    }

    std::vector<AstStatement*> getStatements() const {
        return Tools::toPtrVector(stmts);
    }

    void print(std::ostream& os) const override {
        os << "{" << std::endl;
        for (const auto& stmt : stmts) {
            stmt->print(os);
            os << std::endl;
        }
        os << "}";
    }

private:
    std::vector<std::unique_ptr<AstStatement>> stmts;
};

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
    AstFunctionCall(std::string name) : name(name), args({}) {}

    AstFunctionCall(std::string name,
                    std::vector<std::unique_ptr<AstExpression>> args)
        : name(name), args(std::move(args)) {}

    std::string getName() const { return name; }

    std::vector<AstExpression*> getArguments() const {
        return Tools::toPtrVector(args);
    }

    void addArgument(std::unique_ptr<AstExpression> expr) {
        args.push_back(std::move(expr));
    }

    virtual void print(std::ostream& os) const {
        os << name << "(";

        if (args.empty()) {
            os << "()";
        } else {
            os << "(";
            os << *args[0];
            for (size_t i = 0; i < args.size(); i++) {
                os << ", ";
                os << *args[i];
            }
            os << ")";
        }
    }

private:
    std::string name;
    std::vector<std::unique_ptr<AstExpression>> args;
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

class AstCondition : public AstNode {
public:
    AstCondition() {}
};

class AstBinaryComparison : public AstCondition {
public:
    AstBinaryComparison(std::string op, std::unique_ptr<AstExpression> lhs, std::unique_ptr<AstExpression> rhs) : op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}

    AstExpression* getLHS() const {
        return lhs.get();
    }

    AstExpression* getRHS() const {
        return rhs.get();
    }

    std::string getOperator() const {
        return op;
    }

    void print(std::ostream& os) const override {
        os << "(";
        lhs->print(os);
        os << " " << op << " ";
        rhs->print(os);
        os << ")";
    };
private:
    std::string op;
    std::unique_ptr<AstExpression> lhs;
    std::unique_ptr<AstExpression> rhs;
};

class AstConjunction : public AstCondition {
public:
    AstConjunction(std::unique_ptr<AstCondition> lhs, std::unique_ptr<AstCondition> rhs) : lhs(std::move(lhs)), rhs(std::move(rhs)) {}

    void print(std::ostream& os) const override {
        os << "(";
        lhs->print(os);
        os << " && ";
        rhs->print(os);
        os << ")";
    }

private:
    std::unique_ptr<AstCondition> lhs;
    std::unique_ptr<AstCondition> rhs;
};

class AstDisjunction : public AstCondition {
public:
    AstDisjunction(std::unique_ptr<AstCondition> lhs, std::unique_ptr<AstCondition> rhs) : lhs(std::move(lhs)), rhs(std::move(rhs)) {}

    void print(std::ostream& os) const override {
        os << "(";
        lhs->print(os);
        os << " || ";
        rhs->print(os);
        os << ")";
    }

private:
    std::unique_ptr<AstCondition> lhs;
    std::unique_ptr<AstCondition> rhs;
};

class AstTrue : public AstCondition {
public:
    AstTrue() {}

    void print(std::ostream& os) const override { os << "true"; }
};

class AstFalse : public AstCondition {
public:
    AstFalse() {}

    void print(std::ostream& os) const override { os << "false"; }
};

class AstConditional : public AstStatement {
public:
    AstConditional(std::unique_ptr<AstCondition> cond)
        : cond(std::move(cond)) {}

    AstCondition* getCondition() const { return cond.get(); }

protected:
    std::unique_ptr<AstCondition> cond;
};

class AstSimpleConditional : public AstConditional {
public:
    AstSimpleConditional(std::unique_ptr<AstCondition> cond,
                         std::unique_ptr<AstStatement> ifStmt)
        : AstConditional(std::move(cond)), ifStmt(std::move(ifStmt)) {}

    AstStatement* getIfBranch() const {
        return ifStmt.get();
    }

    void print(std::ostream& os) const override {
        os << "if (";
        cond->print(os);
        os << ") ";
        ifStmt->print(os);
    }

private:
    std::unique_ptr<AstStatement> ifStmt;
};

class AstBranchingConditional : public AstConditional {
public:
    AstBranchingConditional(std::unique_ptr<AstCondition> cond,
                            std::unique_ptr<AstStatement> ifStmt,
                            std::unique_ptr<AstStatement> elseStmt)
        : AstConditional(std::move(cond)), ifStmt(std::move(ifStmt)),
          elseStmt(std::move(elseStmt)) {}

    AstStatement* getIfBranch() const {
        return ifStmt.get();
    }

    AstStatement* getElseBranch() const {
        return elseStmt.get();
    }

    void print(std::ostream& os) const override {
        os << "if (";
        cond->print(os);
        os << ") ";
        ifStmt->print(os);
        os << "else ";
        elseStmt->print(os);
    }

private:
    std::unique_ptr<AstStatement> ifStmt;
    std::unique_ptr<AstStatement> elseStmt;
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

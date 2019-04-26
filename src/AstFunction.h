#pragma once

#include "AstNode.h"
#include "AstStatement.h"

#include <vector>

class AstFunctionDecl : public AstNode {
public:
    AstFunctionDecl(std::string name) : name(name), args({}), stmts({}) {}

    AstFunctionDecl(std::string name,
                std::vector<std::unique_ptr<AstVariable>> args,
                std::vector<std::unique_ptr<AstStatement>> stmts)
        : name(name), args(std::move(args)), stmts(std::move(stmts)) {}

    std::string getName() const {
        return name;
    }

    std::vector<AstVariable*> getArguments() const {
        std::vector<AstVariable*> result;
        for (const auto& arg : args) {
            result.push_back(arg.get());
        }
        return result;
    }

    std::vector<AstStatement*> getStatements() const {
        std::vector<AstStatement*> result;
        for (const auto& stmt : stmts) {
            result.push_back(stmt.get());
        }
        return result;
    }

    void addArgument(std::unique_ptr<AstVariable> var) {
        args.push_back(std::move(var));
    }

    void addStatement(std::unique_ptr<AstStatement> stmt) {
        stmts.push_back(std::move(stmt));
    }

    virtual void print(std::ostream& os) const {
        os << "func " << name << " ";

        if (args.empty()) {
            os << "()";
        } else {
            os << "(";
            os << *args[0];
            for (size_t i = 1; i < args.size(); i++) {
                os << ", ";
                os << *args[i];
            }
            os << ")";
        }

        os << " {" << std::endl;
        for (const auto& stmt : stmts) {
            os << "\t" << *stmt << ";" << std::endl;
        }
        os << "}";
    }

private:
    std::string name;
    std::vector<std::unique_ptr<AstVariable>> args;
    std::vector<std::unique_ptr<AstStatement>> stmts;
};

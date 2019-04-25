#pragma once

#include "AstVisitor.h"

#include <map>
#include <sstream>

class Translator : public AstVisitor<void> {
public:
    Translator(std::ostream& os, AstProgram* program)
        : os(os), program(program) {}

    void run() { visit(program); }

protected:
    void visitProgram(const AstProgram* program) override {
        os << "!#/bin/bash" << std::endl;
        os << std::endl;

        os << "# global variables" << std::endl;
        for (const auto* assignment : program->getAssignments()) {
            visitAssignment(assignment);
            os << std::endl;
        }
        os << std::endl;

        os << "# functions" << std::endl;
        for (const auto* function : program->getFunctions()) {
            visitFunction(function);
        }
        os << std::endl;
    }

    void visitFunction(const AstFunction* function) override {
        std::string bID = getBashIdentifier(function->getName());
        os << bID << " (" << ") {" << std::endl;
        for (const auto* stmt : function->getStatements()) {
            visit(stmt);
            os << std::endl;
        }
        os << "}" << std::endl;
    }

    void visitAssignment(const AstAssignment* assignment) override {
        std::string pID = assignment->getVariable()->getName();
        std::string bID = getBashIdentifier(pID);
        os << bID << "=";
        visit(assignment->getExpression());
    }

    void visitVariable(const AstVariable* variable) override {
        std::string bID = getBashIdentifier(variable->getName());
        os << "$" << bID;
    }

    void visitNumberLiteral(const AstNumberLiteral* lit) override {
        os << lit->getNumber();
    }

    void visitStringLiteral(const AstStringLiteral* lit) override {
        os << lit->getString();
    }

    void visitBinaryExpression(const AstBinaryExpression* expr) override {
        os << "$((";
        visit(expr->getLHS());
        os << expr->getOperator();
        visit(expr->getRHS());
        os << "))";
    }

private:
    // TODO: add tab level support
    std::ostream& os;
    AstProgram* program;
    std::map<std::string, std::string> identMap;

    std::string getBashIdentifier(std::string punchIdentifier) {
        auto pos = identMap.find(punchIdentifier);
        if (pos != identMap.end()) {
            return pos->second;
        }

        std::string name = generateIdentifier(punchIdentifier);
        identMap[punchIdentifier] = name;
        return name;
    }

    std::string generateIdentifier(std::string punchIdentifier) {
        std::stringstream name(punchIdentifier);
        auto pos = identMap.find(name.str());
        while (pos != identMap.end()) {
            name << "0";
            pos = identMap.find(name.str());
        }
        return name.str();
    }
};

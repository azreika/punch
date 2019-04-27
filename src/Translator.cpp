#include "Translator.h"

void Translator::visitProgram(const AstProgram* program) {
    os << "#!/bin/bash" << std::endl;
    os << std::endl;

    os << "# global variables" << std::endl;
    for (const auto* assignment : program->getAssignments()) {
        visitAssignment(assignment);
        os << std::endl;
    }
    os << std::endl;

    os << "# functions" << std::endl;
    for (const auto* function : program->getFunctions()) {
        visit(function);
    }
    os << std::endl;

    os << "# start the program" << std::endl;
    os << getBashIdentifier("main");
    os << std::endl;
}

void Translator::visitFunctionDecl(const AstFunctionDecl* function) {
    std::string bID = getBashIdentifier(function->getName());
    os << bID << " ("
       << ") {" << std::endl;

    tabLevel += 1;
    for (const auto* stmt : function->getStatements()) {
        os << tabs();
        visit(stmt);
        os << std::endl;
    }
    tabLevel -= 1;

    os << "}" << std::endl;
}

void Translator::visitAssignment(const AstAssignment* assignment) {
    std::string pID = assignment->getVariable()->getName();
    std::string bID = getBashIdentifier(pID);
    os << bID << "=";
    visit(assignment->getExpression());
}

void Translator::visitVariable(const AstVariable* variable) {
    std::string bID = getBashIdentifier(variable->getName());
    os << "$" << bID;
}

void Translator::visitNumberLiteral(const AstNumberLiteral* lit) {
    os << lit->getNumber();
}

void Translator::visitStringLiteral(const AstStringLiteral* lit) {
    os << lit->getString();
}

void Translator::visitBinaryExpression(const AstBinaryExpression* expr) {
    os << "$((";
    visit(expr->getLHS());
    os << expr->getOperator();
    visit(expr->getRHS());
    os << "))";
}

void Translator::visitReturn(const AstReturn* ret) {
    os << "echo ";
    visit(ret->getExpression());
}

void Translator::visitRawBashExpression(const AstRawBashExpression* raw) {
    os << raw->getExpression();
}

void Translator::visitRawPunchExpression(const AstRawPunchExpression* expr) {
    visit(expr->getExpression());
}

void Translator::visitRawEnvironment(const AstRawEnvironment* env) {
    for (auto* expr : env->getExpressions()) {
        visit(expr);
    }
}

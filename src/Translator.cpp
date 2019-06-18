#include "Translator.h"

void Translator::visitProgram(const AstProgram* program) {
    os << "#!/bin/bash";
    newLine();
    newLine();

    os << "# global variables";
    newLine();

    for (const auto* assignment : program->getAssignments()) {
        visitAssignment(assignment);
        newLine();
    }
    newLine();

    os << "# functions";

    newLine();
    for (const auto* function : program->getFunctions()) {
        newLine();
        visit(function);
        newLine();
    }
    newLine();

    os << "# start the program";
    newLine();

    os << getBashIdentifier("main");
    newLine();
}

void Translator::visitFunctionDecl(const AstFunctionDecl* function) {
    std::string bID = getBashIdentifier(function->getName());
    os << bID << " ("
       << ") {";
    tabInc();
    for (const auto* stmt : function->getStatements()) {
        newLine();
        visit(stmt);
    }
    tabDec();
    newLine();
    os << "}";
}

void Translator::visitFunctionCall(const AstFunctionCall* call) {
    std::string bID = getBashIdentifier(call->getName());
    os << "$(" << bID;
    if (!call->getArguments().empty()) {
        for (const auto* arg : call->getArguments()) {
            os << " ";
            visit(arg);
        }
    }
    os << ")";
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

void Translator::visitTrue(const AstTrue* val) { os << "true"; }

void Translator::visitFalse(const AstFalse* val) { os << "false"; }

void Translator::visitSimpleConditional(
    const AstSimpleConditional* conditional) {
    os << "if [";
    visit(conditional->getCondition());
    os << "]";
    newLine();
    os << "then";

    tabInc();
    newLine();
    visit(conditional->getIfBranch());
    tabDec();

    newLine();
    os << "fi";
}

void Translator::visitBranchingConditional(
    const AstBranchingConditional* conditional) {
    os << "if [ ";
    visit(conditional->getCondition());
    os << " ]";
    newLine();
    os << "then";

    tabInc();
    newLine();
    visit(conditional->getIfBranch());
    tabDec();

    newLine();
    os << "else";

    tabInc();
    newLine();
    visit(conditional->getElseBranch());
    tabDec();

    newLine();
    os << "fi";
}

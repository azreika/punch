#include "Translator.h"

void Translator::visitProgram(const AstProgram* program) {
    os << "#!/bin/bash";
    newLine();
    newLine();

    if (!program->getAssignments().empty()) {
        os << "# global variables";
        newLine();
        for (const auto* assignment : program->getAssignments()) {
            visitAssignment(assignment);
            newLine();
        }
        newLine();
    }

    if (!program->getFunctions().empty()) {
        os << "# functions";
        newLine();
        for (const auto* function : program->getFunctions()) {
            visit(function);
            newLine();
            newLine();
        }
    }

    os << "# start the program";
    newLine();

    os << getBashIdentifier("main");
    newLine();
}

void Translator::visitFunctionDecl(const AstFunctionDecl* function) {
    std::string bID = getBashIdentifier(function->getName());
    os << bID << " () {";

    tabInc();

    size_t argCount = 0;
    for (const auto* arg : function->getArguments()) {
        newLine();
        std::string argID = getBashIdentifier(arg->getName());
        os << argID << "=$" << ++argCount;
    }

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
    os << "if $(";
    visit(conditional->getCondition());
    os << ")";
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
    os << "if $(";
    visit(conditional->getCondition());
    os << ")";
    newLine();
    os << "then";

    tabInc();
    newLine();
    visit(conditional->getIfBranch());
    tabDec();

    newLine();
    const auto* elseBranch = conditional->getElseBranch();
    if (dynamic_cast<const AstConditional*>(elseBranch) != nullptr) {
        os << "el";
        visit(elseBranch);
    } else {
        os << "else";
        tabInc();
        newLine();
        visit(elseBranch);
        tabDec();
        newLine();
        os << "fi";
    }
}

void Translator::visitStatementBlock(const AstStatementBlock* stmtBlock) {
    os << "{";

    tabInc();
    for (const auto* stmt : stmtBlock->getStatements()) {
        newLine();
        visit(stmt);
    }
    tabDec();
    newLine();

    os << "}";
}

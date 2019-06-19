#pragma once

#include "AstVisitor.h"

#include <map>
#include <sstream>

class Translator : public AstVisitor<void> {
public:
    Translator(std::ostream& os, AstProgram* program)
        : os(os), program(program), identMap({}), tabLevel(0) {}

    void run() { visit(program); }

protected:
    void visitProgram(const AstProgram*) override;
    void visitFunctionDecl(const AstFunctionDecl*) override;
    void visitFunctionCall(const AstFunctionCall*) override;
    void visitAssignment(const AstAssignment*) override;
    void visitVariable(const AstVariable*) override;
    void visitNumberLiteral(const AstNumberLiteral*) override;
    void visitStringLiteral(const AstStringLiteral*) override;
    void visitBinaryExpression(const AstBinaryExpression*) override;
    void visitReturn(const AstReturn*) override;
    void visitRawBashExpression(const AstRawBashExpression*) override;
    void visitRawPunchExpression(const AstRawPunchExpression*) override;
    void visitRawEnvironment(const AstRawEnvironment*) override;
    void visitTrue(const AstTrue*) override;
    void visitFalse(const AstFalse*) override;
    void visitSimpleConditional(const AstSimpleConditional*) override;
    void visitBranchingConditional(const AstBranchingConditional*) override;
    void visitStatementBlock(const AstStatementBlock*) override;

private:
    std::ostream& os;
    AstProgram* program;
    std::map<std::string, std::string> identMap;
    size_t tabLevel;

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

    static std::string generateVariable() {
        static size_t count = 0;
        std::stringstream name;
        name << "_internal_" << count++;
        return name.str();
    }

    std::string tabs() const {
        std::stringstream tabs;
        for (size_t i = 0; i < tabLevel; i++) {
            tabs << "    ";
        }
        return tabs.str();
    }

    void indent() {
        os << tabs();
    }

    void tabInc() {
        tabLevel++;
    }

    void tabDec() {
        tabLevel--;
    }

    void newLine() {
        os << std::endl;
        indent();
    }
};

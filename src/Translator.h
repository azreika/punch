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
    void visitAssignment(const AstAssignment*) override;
    void visitVariable(const AstVariable*) override;
    void visitNumberLiteral(const AstNumberLiteral*) override;
    void visitStringLiteral(const AstStringLiteral*) override;
    void visitBinaryExpression(const AstBinaryExpression*) override;
    void visitReturn(const AstReturn*) override;
    void visitRawBashExpression(const AstRawBashExpression*) override;
    void visitRawPunchExpression(const AstRawPunchExpression*) override;
    void visitRawEnvironment(const AstRawEnvironment*) override;

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

    std::string tabs() const {
        std::stringstream tabs;
        for (size_t i = 0; i < tabLevel; i++) {
            tabs << "    ";
        }
        return tabs.str();
    }
};

#include "AstVisitor.h"

class Translator : public AstVisitor<void> {
public:
    Translator(std::ostream& os, AstProgram* program)
        : os(os), program(program) {}

    void run() { visit(program); }

    void visitProgram(const AstProgram* program) override {
        os << "!#/bin/bash" << std::endl;
        os << std::endl;

        os << "# global variables" << std::endl;
        for (const auto* assignment : program->getAssignments()) {
            visitAssignment(assignment);
        }
        os << std::endl;

        os << "# functions" << std::endl;
        for (const auto* function : program->getFunctions()) {
            visitFunction(function);
        }
        os << std::endl;
    }

    void visitFunction(const AstFunction* function) override {
        os << "# TODO: define function " << function->getName() << "..."
           << std::endl;
    }

    void visitAssignment(const AstAssignment* assignment) override {
        os << "# TODO: define global variable " << assignment->getName() << "..." << std::endl;
    }

private:
    std::ostream& os;
    AstProgram* program;
};

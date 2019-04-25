#include "AstVisitor.h"

class Translator : public AstVisitor {
public:
    Translator(std::ostream& os, AstProgram* program) : os(os), program(program) {}

    void run() {
        visit(program);
    }

    void visitProgram(const AstProgram* program) {
        os << "PROGRAM!" << std::endl;
    }

private:
    std::ostream& os;
    AstProgram* program;
};

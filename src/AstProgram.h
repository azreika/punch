#pragma once

#include "AstFunction.h"
#include "AstNode.h"
#include "AstStatement.h"
#include "Tools.h"

#include <iostream>
#include <memory>
#include <vector>

class AstProgram : public AstNode {
public:
    void print(std::ostream& os) const override {
        os << "// assignments" << std::endl;
        os << std::endl;
        for (const auto& assignment : assignments) {
            assignment->print(os);
            os << ";" << std::endl;
        }

        os << std::endl;

        os << "// functions" << std::endl;
        os << std::endl;
        for (const auto& function : functions) {
            function->print(os);
            os << std::endl;
        }
    }

    std::vector<AstAssignment*> getAssignments() const {
        return Tools::toPtrVector(assignments);
    }

    std::vector<AstFunctionDecl*> getFunctions() const {
        return Tools::toPtrVector(functions);
    }

    void addAssignment(std::unique_ptr<AstAssignment> assignment) {
        assignments.push_back(std::move(assignment));
    }

    void addFunction(std::unique_ptr<AstFunctionDecl> function) {
        functions.push_back(std::move(function));
    }

private:
    std::vector<std::unique_ptr<AstAssignment>> assignments;
    std::vector<std::unique_ptr<AstFunctionDecl>> functions;
};

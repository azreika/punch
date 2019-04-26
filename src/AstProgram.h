#pragma once

#include "AstFunction.h"
#include "AstNode.h"
#include "AstStatement.h"

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

    // TODO: helper function for unique_ptr to pointer

    std::vector<AstAssignment*> getAssignments() const {
        std::vector<AstAssignment*> result;
        for (const auto& assignment : assignments) {
            result.push_back(assignment.get());
        }
        return result;
    }

    std::vector<AstFunctionDecl*> getFunctions() const {
        std::vector<AstFunctionDecl*> result;
        for (const auto& function : functions) {
            result.push_back(function.get());
        }
        return result;
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

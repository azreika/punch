#pragma once

#include "AstNode.h"
#include "AstFunction.h"
#include "AstStatement.h"

#include <iostream>
#include <vector>
#include <memory>

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

    void addAssignment(std::unique_ptr<AstAssignment> assignment) {
        assignments.push_back(std::move(assignment));
    }

    void addFunction(std::unique_ptr<AstFunction> function) {
        functions.push_back(std::move(function));
    }

private:
    std::vector<std::unique_ptr<AstAssignment>> assignments;
    std::vector<std::unique_ptr<AstFunction>> functions;
};


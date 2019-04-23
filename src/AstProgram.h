#pragma once

#include "AstNode.h"
#include "AstStatement.h"

#include <iostream>
#include <vector>
#include <memory>

class AstProgram : public AstNode {
public:
    void print(std::ostream& os) const override {
        for (const auto& assignment : assignments) {
            assignment->print(os);
            os << ";" << std::endl;
        }
    }

    void addAssignment(std::unique_ptr<AstAssignment> assignment) {
        assignments.push_back(std::move(assignment));
    }

private:
    std::vector<std::unique_ptr<AstAssignment>> assignments;
};


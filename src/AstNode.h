#pragma once

#include <iostream>

class AstNode {
public:
    virtual void print(std::ostream& os) const = 0;

    friend std::ostream& operator<<(std::ostream& os, const AstNode& node) {
        node.print(os);
        return os;
    }
};

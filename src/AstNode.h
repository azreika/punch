#pragma once

#include <iostream>

class AstNode {
    friend std::ostream& operator<<(std::ostream& os, const AstNode& node) {
        os << "NODE";
        return os;
    }
};

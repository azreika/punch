#pragma once

#include "AstNode.h"
#include "Token.h"

#include <vector>

class Parser {
public:
    Parser(const std::vector<Token>& tokens) : idx(0), tokens(tokens) {}

    AstNode* parse() { return parseProgram(); }

private:
    int idx;
    const std::vector<Token>& tokens;

    AstNode* parseProgram();
};

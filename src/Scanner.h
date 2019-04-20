#pragma once

#include "Token.h"

#include <vector>

class Scanner {
public:
    Scanner(std::string source) : source(source), idx(0), tokens({}) {
        scanTokens();
    }

    const std::vector<Token>& getTokens() const {
        return tokens;
    }

private:
    std::string source;
    size_t idx;
    std::vector<Token> tokens;

    void scanTokens();

    void addToken(TokenType type);
};

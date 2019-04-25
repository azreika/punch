#pragma once

#include "AstProgram.h"
#include "AstStatement.h"
#include "Token.h"

#include <vector>

class Parser {
public:
    Parser(const std::vector<Token>& tokens) : idx(0), tokens(tokens) {}

    AstProgram* parse() { return parseProgram(); }

    bool hasNext() { return peek().type != TokenType::END; }

    Token advance() { return tokens[idx++]; }

    Token peek() const { return peek(0); }

    Token peek(size_t count) const {
        if (idx + count >= tokens.size()) {
            return TokenType::END;
        }
        return tokens[idx + count];
    }

    bool match(TokenType type) {
        if (peek().type == type) {
            advance();
            return true;
        } else {
            return false;
        }
    }

private:
    int idx;
    const std::vector<Token>& tokens;

    AstProgram* parseProgram();

    AstAssignment* parseAssignment();

    AstExpression* parseExpression();
    AstExpression* parseTerm();
    AstExpression* parseFactor();

    AstFunction* parseFunction();

    AstStatement* parseStatement();

    AstRawEnvironment* parseRawEnvironment();
};

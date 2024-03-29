#pragma once

#include "AstProgram.h"
#include "AstStatement.h"
#include "PunchException.h"
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
            return Token(TokenType::END, 0, 0);
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

    AstFunctionDecl* parseFunction();

    AstStatement* parseStatement();

    AstStatementBlock* parseStatementBlock();

    AstConditional* parseConditional();

    AstCondition* parseCondition();

    AstRawEnvironment* parseRawEnvironment();

    // TODO: clean up error generation
    void generateError(Token seen, std::vector<TokenType> expected) {
        ParserException exc =
            expected.empty()
                ? ParserException(seen.type, seen.line, seen.col)
                : ParserException(seen.type, expected, seen.line, seen.col);
        PunchException::handleException(exc);
        exit(1);
    }
};

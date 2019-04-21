#pragma once

#include "Token.h"

#include <vector>

class Scanner {
public:
    Scanner(std::string source)
        : source(source), idx(0), currTokenStart(0), tokens({}) {
        run();
    }

    bool hasNext() const { return idx < source.length(); }

    const std::vector<Token>& getTokens() const { return tokens; }

private:
    std::string source;
    size_t idx;
    size_t currTokenStart;
    std::vector<Token> tokens;

    void run();

    char advance() {
        return source[idx++];
    }

    char peek() {
        return source[idx];
    }

    bool match(char chr) {
        if (chr == peek()) {
            advance();
            return true;
        } else {
            return false;
        }
    }

    void expect(char chr) {
        if (chr != peek()) {
            std::cout << chr << " BUT GOT " << peek() << std::endl;
            assert(false && "expected another character");
        }
        advance();
    }

    void scanToken();

    void scanString();

    void scanIdentifier();

    void scanNumber();

    void scanComment();

    void scanRawEnvironment(char start, char end);

    void addToken(TokenType type) {
        tokens.push_back(Token(type));
    }

    void addToken(TokenType type, std::string stringLiteral) {
        tokens.push_back(Token(type, stringLiteral));
    }

    void addToken(TokenType type, int numberLiteral) {
        tokens.push_back(Token(type, numberLiteral));
    }
};

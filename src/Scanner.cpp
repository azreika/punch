#include "Scanner.h"
#include "Token.h"

#include <vector>

void Scanner::scanTokens() {
    addToken(TokenType::END);
}

void Scanner::addToken(TokenType type) {
    tokens.push_back(Token(type));
}

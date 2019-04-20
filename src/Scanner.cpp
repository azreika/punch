#include "Scanner.h"
#include "Token.h"

#include <vector>

void Scanner::scanToken() {
    char chr = advance();
    switch (chr) {
        // whitespace
        case ' ':
        case '\t':
        case '\n':
        case '\r': break;

        // single-character simple tokens
        case '(': addToken(TokenType::LPAREN); break;
        case ')': addToken(TokenType::RPAREN); break;
        case '{': addToken(TokenType::LBRACE); break;
        case '}': addToken(TokenType::RBRACE); break;
        case ';': addToken(TokenType::SEMICOLON); break;
        case '+': addToken(TokenType::PLUS); break;
        case '-': addToken(TokenType::MINUS); break;
        case '*': addToken(TokenType::STAR); break;
        case '[': addToken(TokenType::LBRACKET); break;
        case ']': addToken(TokenType::RBRACKET); break;

        // possibly multi-character simple tokens
        case '=': {
            if (match('=')) {
                addToken(TokenType::EQUALEQUAL);
            } else {
                addToken(TokenType::EQUAL);
            }
            break;
        }

        case '<': {
            if (match('=')) {
                addToken(TokenType::LEQ);
            } else {
                addToken(TokenType::LESSTHAN);
            }
            break;
        }

        case '>': {
            if (match('>')) {
                addToken(TokenType::GEQ);
            } else {
                addToken(TokenType::GREATERTHAN);
            }
            break;
        }

        case '!': {
            if (match('=')) {
                addToken(TokenType::NOTEQUAL);
            } else {
                addToken(TokenType::LNOT);
            }
            break;
        }

        // complex tokens
        case '"': scanString(); break;
        case '$': {
            scanRawEnvironment('(', ')');
            break;
        }

        default: {
            if (isdigit(chr)) {
                scanNumber();
            } else if (isalpha(chr)) {
                scanIdentifier();
            } else {
                assert(false && "unexpected character during token scan");
            }
        }
    }
}

void Scanner::scanString() {
    while (hasNext() && peek() != '"') {
        advance();
    }
    advance();

    std::string result = source.substr(currTokenStart+1, idx-currTokenStart-1);
    std::cout << result << std::endl;
    addToken(TokenType::STRING, result);
}

void Scanner::scanIdentifier() {
    while (hasNext() && isalnum(peek())) {
        advance();
    }

    std::string result = source.substr(currTokenStart, idx - currTokenStart);
    if (result == "var") {
        addToken(TokenType::VAR);
    } else if (result == "func") {
        addToken(TokenType::FUNC);
    } else if (result == "if") {
        addToken(TokenType::IF);
    } else if (result == "else") {
        addToken(TokenType::ELSE);
    } else if (result == "for") {
        addToken(TokenType::FOR);
    } else if (result == "while") {
        addToken(TokenType::WHILE);
    } else if (result == "$") {
        addToken(TokenType::DOLLAR);
    } else if (result == "raw") {
        scanRawEnvironment('{', '}');
    } else {
        addToken(TokenType::IDENT, result);
    }
}

void Scanner::scanNumber() {
    while (hasNext() && isdigit(peek())) {
        advance();
    }

    std::string result = source.substr(currTokenStart, idx - currTokenStart);
    int number = std::stoi(result);
    addToken(TokenType::NUMBER, number);
}

void Scanner::scanComment() {
    assert(false && "unimplemented");
}

void Scanner::scanRawEnvironment(char start, char end) {
    while (hasNext() && peek() != start) {
        advance();
    }
    advance();
    int startIdx = idx;

    int nestingLevel = 1;
    while (hasNext()) {
        if (peek() == start) {
            nestingLevel++;
        } else if (peek() == end) {
            nestingLevel--;
            if (nestingLevel == 0) {
                advance();
                break;
            }
        }
        advance();
    }

    std::string result = source.substr(startIdx, idx - startIdx - 1);
    addToken(TokenType::RAW, result);
}

void Scanner::run() {
    assert(tokens.empty() && "scanner has already been run");
    while (hasNext()) {
        currTokenStart = idx;
        scanToken();
    }
    addToken(TokenType::END);
}

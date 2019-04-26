#pragma once

#include "Token.h"

#include <exception>
#include <iostream>
#include <sstream>
#include <vector>

class PunchException : public std::exception {
public:
    static void handleException(const PunchException& e);

    virtual std::string getMessage() const = 0;
};

class ScannerException : public PunchException {
public:
    ScannerException(std::string msg) : msg(msg) {}

    ScannerException(char charSeen, char charExpected) {
        std::stringstream msgStream;
        msgStream << "expected '" << charExpected << "' but got '" << charSeen
                  << "'";
        msg = msgStream.str();
    }

    ScannerException(char unexpectedChar, size_t line = 0, size_t col = 0) {
        std::stringstream msgStream;
        msgStream << "unexpected character '" << unexpectedChar << "'";
        if (line != 0 && col != 0) {
            msgStream << " on line " << line << ", column " << col;
        }
        msg = msgStream.str();
    }

    virtual std::string getMessage() const { return msg; }

    const char* what() const throw() { return msg.c_str(); }

private:
    std::string msg;
};

class ParserException : public PunchException {
public:
    ParserException(std::string msg) : msg(msg) {}

    // TODO: helper function for join
    ParserException(TokenType tokenSeen, std::vector<TokenType> expectedTokens,
                    size_t line = 0, size_t col = 0) {
        assert(!expectedTokens.empty() &&
               "at least one token type should be expected");
        std::stringstream msgStream;
        msgStream << "expected ";
        for (size_t i = 0; i < expectedTokens.size() - 1; i++) {
            const auto& tok = expectedTokens[i];
            msgStream << "'" << getSymbolForTokenType(tok) << "', ";
        }
        const auto& lastToken = expectedTokens[expectedTokens.size() - 1];
        msgStream << "'" << getSymbolForTokenType(lastToken) << "' ";
        msgStream << "but got '" << getSymbolForTokenType(tokenSeen) << "'";
        if (line != 0 && col != 0) {
            msgStream << " on line " << line << ", column " << col;
        }
        msg = msgStream.str();
    }

    ParserException(TokenType unexpectedToken, size_t line = 0,
                    size_t col = 0) {
        std::stringstream msgStream;
        msgStream << "unexpected token '"
                  << getSymbolForTokenType(unexpectedToken) << "'";
        if (line != 0 && col != 0) {
            msgStream << " on line " << line << ", column " << col;
        }
        msg = msgStream.str();
    }

    virtual std::string getMessage() const { return msg; }

    const char* what() const throw() { return msg.c_str(); }

private:
    std::string msg;
};

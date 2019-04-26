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

    ScannerException(char unexpectedChar) {
        std::stringstream msgStream;
        msgStream << "unexpected character '" << unexpectedChar << "'";
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
    ParserException(TokenType tokenSeen, std::vector<TokenType> expectedTokens) {
        assert(!expectedTokens.empty() && "at least one token type should be expected");
        std::stringstream msgStream;
        msgStream << "expected ";
        for (size_t i = 0; i < expectedTokens.size() - 1; i++) {
            const auto& tok = expectedTokens[i];
            msgStream << "'" << getSymbolForTokenType(tok) << "', ";
        }
        const auto& lastToken = expectedTokens[expectedTokens.size()-1];
        msgStream << "'" << getSymbolForTokenType(lastToken) << "' ";
        msgStream << "but got '" << getSymbolForTokenType(tokenSeen) << "'";
        msg = msgStream.str();
    }

    ParserException(TokenType unexpectedToken) {
        std::stringstream msgStream;
        msgStream << "unexpected token '"
                  << getSymbolForTokenType(unexpectedToken) << "'";
        msg = msgStream.str();
    }

    virtual std::string getMessage() const { return msg; }

    const char* what() const throw() { return msg.c_str(); }

private:
    std::string msg;
};

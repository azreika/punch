#pragma once

#include "Token.h"

#include <exception>
#include <iostream>
#include <sstream>

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

    ParserException(TokenType tokenSeen, TokenType tokenExpected) {
        std::stringstream msgStream;
        msgStream << "expected '" << getSymbolForTokenType(tokenExpected)
                  << "' but got '" << getSymbolForTokenType(tokenSeen) << "'";
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

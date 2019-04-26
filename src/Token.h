#pragma once

#include <cassert>
#include <iostream>
#include <optional>
#include <string>

enum class TokenType {
    // separators
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    LBRACKET,
    RBRACKET,
    SEMICOLON,
    COMMA,

    // keywords
    FUNC,
    IF,
    ELSE,
    FOR,
    WHILE,
    DOLLAR,
    RAW,
    VAR,
    EQUAL,
    RETURN,

    // operators
    LOR,
    LAND,
    LNOT,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    LEQ,
    GEQ,
    EQUALEQUAL,
    NOTEQUAL,
    LESSTHAN,
    GREATERTHAN,

    // literals
    IDENT,
    NUMBER,
    STRING,
    RAWEXPR,

    // EOF
    END,
};

inline std::string getSymbolForTokenType(TokenType type) {
    switch (type) {
        // separators
        case TokenType::LPAREN: return "(";
        case TokenType::RPAREN: return ")";
        case TokenType::LBRACE: return "{";
        case TokenType::RBRACE: return "}";
        case TokenType::LBRACKET: return "[";
        case TokenType::RBRACKET: return "]";
        case TokenType::SEMICOLON: return ";";
        case TokenType::COMMA: return ",";

        // keywords
        case TokenType::FUNC: return "FUNC";
        case TokenType::IF: return "IF";
        case TokenType::ELSE: return "ELSE";
        case TokenType::FOR: return "FOR";
        case TokenType::WHILE: return "WHILE";
        case TokenType::DOLLAR: return "$";
        case TokenType::RAW: return "RAW";
        case TokenType::VAR: return "VAR";
        case TokenType::EQUAL: return "=";
        case TokenType::RETURN: return "RETURN";

        // operators
        case TokenType::LOR: return "||";
        case TokenType::LAND: return "&&";
        case TokenType::LNOT: return "!";
        case TokenType::PLUS: return "+";
        case TokenType::MINUS: return "-";
        case TokenType::STAR: return "*";
        case TokenType::SLASH: return "/";
        case TokenType::LEQ: return "<=";
        case TokenType::GEQ: return ">=";
        case TokenType::EQUALEQUAL: return "==";
        case TokenType::NOTEQUAL: return "!=";
        case TokenType::LESSTHAN: return "<";
        case TokenType::GREATERTHAN: return ">";

        // literals
        case TokenType::IDENT: return "IDENT";
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::STRING: return "STRING";
        case TokenType::RAWEXPR: return "RAWEXPR";

        // EOF
        case TokenType::END: return "EOF";

        // error
        default: break;
    }

    assert(false && "unsupported token type");
}

class Token {
public:
    const TokenType type;
    size_t line;
    size_t col;

    Token(TokenType type, size_t line, size_t col)
        : type(type), line(line), col(col), numberLiteral({}),
          stringLiteral({}) {}

    Token(TokenType type, int numberLiteral, size_t line, size_t col)
        : type(type), line(line), col(col), numberLiteral(numberLiteral),
          stringLiteral({}) {}

    Token(TokenType type, std::string stringLiteral, size_t line, size_t col)
        : type(type), line(line), col(col), numberLiteral({}),
          stringLiteral(stringLiteral) {}

    bool isLiteral() const { return isNumberLiteral() || isStringLiteral(); }

    bool isNumberLiteral() const { return numberLiteral.has_value(); }

    bool isStringLiteral() const { return stringLiteral.has_value(); }

    int getNumberLiteral() const {
        assert(isNumberLiteral() && "token does not contain number literal");
        return numberLiteral.value();
    }

    std::string getStringLiteral() const {
        assert(isStringLiteral() && "token does not contain string literal");
        return stringLiteral.value();
    }

    friend std::ostream& operator<<(std::ostream& os, const Token& token) {
        os << getSymbolForTokenType(token.type);
        if (token.isStringLiteral()) {
            os << "(" << token.getStringLiteral() << ")";
        }
        if (token.isNumberLiteral()) {
            os << "(" << token.getNumberLiteral() << ")";
        }
        return os;
    }

private:
    const std::optional<int> numberLiteral;
    const std::optional<std::string> stringLiteral;
};

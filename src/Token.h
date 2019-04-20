#include <cassert>
#include <optional>
#include <string>

enum class TokenType {
    // separators
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    SEMICOLON,

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
    LESSTHAN,
    GREATERTHAN,

    // literals
    IDENT,
    NUMBER,
    STRING,

    // EOF
    END,
};

class Token {
public:
    const TokenType type;

    Token(TokenType type) : type(type), numberLiteral({}), stringLiteral({}) {}

    Token(TokenType type, int numberLiteral)
        : type(type), numberLiteral(numberLiteral), stringLiteral({}) {}

    Token(TokenType type, std::string stringLiteral)
        : type(type), numberLiteral({}), stringLiteral(stringLiteral) {}

    bool isLiteral() const { return isNumberLiteral() || isStringLiteral(); }

    bool isNumberLiteral() const { return numberLiteral.has_value(); }

    bool isStringLiteral() const { return stringLiteral.has_value(); }

    int getNumber() const {
        assert(isNumberLiteral() && "token does not contain number literal");
        return numberLiteral.value();
    }

    std::string getString() const {
        assert(isStringLiteral() && "token does not contain string literal");
        return stringLiteral.value();
    }

private:
    const std::optional<int> numberLiteral;
    const std::optional<std::string> stringLiteral;
};

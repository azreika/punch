#pragma once

#include "PunchException.h"
#include "Token.h"

#include <vector>

class Scanner {
public:
    Scanner(std::string source)
        : source(source), idx(0), currTokenStart(0), tokens({}), line(1),
          col(0) {
        while (hasNext()) {
            currTokenStart = idx;
            scanToken();
        }
        addToken(TokenType::END);
    }

    /**
     * Checks whether any tokens remain to be scanned.
     *
     * @return true iff any more tokens can be scanned
     */
    bool hasNext() const { return idx < source.length(); }

    /**
     * Gets the final list of tokens from the scanner.
     *
     * @return a vector of scanned tokens
     */
    const std::vector<Token>& getTokens() const { return tokens; }

private:
    std::string source;
    size_t idx;
    size_t currTokenStart;
    std::vector<Token> tokens;
    size_t line;
    size_t col;

    /**
     * Advances the scanner by one character.
     *
     * @return the character that was pointed to by the scanner
     */
    char advance() {
        col += 1;
        return source[idx++];
    }

    /**
     * Gets the current character in the source string, without advancing the
     * scanner.
     *
     * @return the current character pointed to by the scanner
     */
    char peek() { return source[idx]; }

    /**
     * Advances the scanner iff the current character matches the expected
     * character.
     *
     * @param chr the expected character
     * @return true iff the scanner has been advanced
     */
    bool match(char chr) {
        if (chr == peek()) {
            advance();
            return true;
        } else {
            return false;
        }
    }

    /**
     * Scans the next token in the source string.
     */
    void scanToken();

    /**
     * Scans the next number token in the source string.
     */
    void scanNumber();

    /**
     * Scans the next string token in the source string.
     *
     * @note assumes the first '"' character has already been scanned.
     */
    void scanString();

    /**
     * Scans the next identifier or keyword in the source string.
     */
    void scanIdentifier();

    /**
     * Scans the next comment sequence in the source string, discarding it.
     */
    void scanComment();

    /**
     * Scans in the tokens in a raw environment.
     *
     * @param start the start marker of the raw environment
     * @param end the end marker of the raw environment
     *
     * @note nested start/end pairs are permitted within the raw environment
     */
    void scanRawEnvironment(char start, char end);

    /**
     * Adds a token to the token stream.
     *
     * @param type the type of the token to push in
     */
    void addToken(TokenType type) { tokens.push_back(Token(type, line, col)); }

    /**
     * Adds a string-literal token to the token stream.
     *
     * @param type the type of the token to push in
     * @stringLiteral the string literal attached to the token
     */
    void addToken(TokenType type, std::string stringLiteral) {
        tokens.push_back(Token(type, stringLiteral, line, col));
    }

    /**
     * Adds a number-literal token to the token stream.
     *
     * @param type the type of the token to push in
     * @numberLiteral the number literal attached to the token
     */
    void addToken(TokenType type, int numberLiteral) {
        tokens.push_back(Token(type, numberLiteral, line, col));
    }

    /**
     * Generates an error when an unexpected character appears.
     *
     * @param seen the unexpected character to report
     * @param line the line the character appeared on
     * @param col the column of the character within the line
     */
    void generateError(char seen, size_t line, size_t col) {
        PunchException::handleException(ScannerException(seen, line, col));
        // TODO: non-breaking error handling
        exit(1);
    }
};

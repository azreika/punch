#include "Scanner.h"
#include "Token.h"

#include <vector>

void Scanner::scanToken() {
    char chr = advance();

    switch (chr) {
        // whitespace
        case '\n': line += 1; col = 0;
        case ' ':
        case '\t':
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
        case '%': addToken(TokenType::PERCENT); break;
        case '[': addToken(TokenType::LBRACKET); break;
        case ']': addToken(TokenType::RBRACKET); break;
        case ',': addToken(TokenType::COMMA); break;

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

        case '/': {
            if (peek() == '/' || peek() == '*') {
                scanComment();
            } else {
                addToken(TokenType::SLASH);
            }
            break;
        }

        // complex tokens
        case '"': scanString(); break;
        case '$': {
            addToken(TokenType::DOLLAR);
            scanRawEnvironment('(', ')');
            break;
        }

        default: {
            if (isdigit(chr)) {
                scanNumber();
            } else if (isalpha(chr)) {
                scanIdentifier();
            } else {
                // unexpected character
                generateError(chr, line, col);
            }
        }
    }
}

// TODO: handle underscores as part of identifiers, etc.

void Scanner::scanString() {
    // keep going until we hit the end of the string
    while (hasNext() && peek() != '"') {
        advance();
    }

    // read in the final '"' character
    advance();

    // the string is everything except the surrounding '"' characters
    std::string result =
        source.substr(currTokenStart + 1, idx - currTokenStart - 1);
    addToken(TokenType::STRING, result);
}

void Scanner::scanIdentifier() {
    // keep reading in contiguous alphanumeric characters
    // TODO: check that isalnum is what is expected
    while (hasNext() && isalnum(peek())) {
        advance();
    }

    std::string result = source.substr(currTokenStart, idx - currTokenStart);

    // match with a keyword if possible
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
        addToken(TokenType::RAW);
        scanRawEnvironment('{', '}');
    } else if (result == "return") {
        addToken(TokenType::RETURN);
    } else if (result == "true") {
        addToken(TokenType::TRUEVAL);
    } else if (result == "false") {
        addToken(TokenType::FALSEVAL);
    } else {
        // otherwise, it is an identifier
        addToken(TokenType::IDENT, result);
    }
}

void Scanner::scanNumber() {
    // keep reading in all digits
    while (hasNext() && isdigit(peek())) {
        advance();
    }

    std::string result = source.substr(currTokenStart, idx - currTokenStart);
    int number = std::stoi(result);
    addToken(TokenType::NUMBER, number);
}

void Scanner::scanComment() {
    // TODO: assumes first char already scnaned
    if (match('/')) {
        while (hasNext() && peek() != '\n') {
            advance();
        }
    } else if (match('*')) {
        // TODO: support nested multiline comments
        while (hasNext()) {
            if (match('*')) {
                if (peek() == '/') {
                    return;
                }
            } else {
                advance();
            }
        }
    }
}

void Scanner::scanRawEnvironment(char start, char end) {
    // TODO: currently ignoring everything until start character; fix this
    while (hasNext() && peek() != start) {
        advance();
    }
    assert(advance() == start && "expected start symbol");

    // add the start token
    switch (start) {
        case '{': addToken(TokenType::LBRACE); break;
        case '(': addToken(TokenType::LPAREN); break;
        default: assert(false && "unexpected start of raw environment");
    }

    int startIdx = idx;
    int nestingLevel = 1;

    while (hasNext()) {
        char chr = advance();
        if (chr == start) {
            nestingLevel++;
        } else if (chr == end) {
            nestingLevel--;
            if (nestingLevel == 0) {
                // finished with the raw environment!
                break;
            }
        } else if (chr == '$' && match('[')) {
            // hit a nested punch expression!

            // add everything read so far as a block of raw expressions
            // '$[' should be removed
            std::string result = source.substr(startIdx, idx - startIdx - 2);
            addToken(TokenType::RAWEXPR, result);

            // add in the '$[' tokens
            addToken(TokenType::DOLLAR);
            addToken(TokenType::LBRACKET);

            // keep scanning in tokens as if in a regular punch environment,
            // until the nested expression is terminated (with a ']')
            Token* token = &tokens[tokens.size() - 1];
            while (token->type != TokenType::RBRACKET) {
                currTokenStart = idx;
                scanToken();
                token = &tokens[tokens.size() - 1];
            }

            // raw block now starts from current index
            startIdx = idx;
        }
    }

    // add in the final raw expression block
    // end character should be ignored
    std::string result = source.substr(startIdx, idx - startIdx - 1);
    addToken(TokenType::RAWEXPR, result);

    // add in the end token
    switch (end) {
        case '}': addToken(TokenType::RBRACE); break;
        case ')': addToken(TokenType::RPAREN); break;
        default: assert(false && "unexpected end of raw environment");
    }
}

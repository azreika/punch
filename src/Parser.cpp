#include "Parser.h"
#include "AstNode.h"
#include "AstStatement.h"
#include "Token.h"

// TODO: error handling

AstProgram* Parser::parseProgram() {
    AstProgram* program = new AstProgram();

    while (hasNext()) {
        if (peek().type == TokenType::FUNC) {
            auto function = std::unique_ptr<AstFunction>(parseFunction());
            program->addFunction(std::move(function));
        } else if (peek().type == TokenType::VAR ||
                   peek().type == TokenType::IDENT) {
            auto assignment = std::unique_ptr<AstAssignment>(parseAssignment());
            program->addAssignment(std::move(assignment));
        } else {
            assert(false && "unexpected token");
        }
    }

    return program;
}

AstAssignment* Parser::parseAssignment() {
    if (match(TokenType::VAR)) {
        std::string ident = advance().getStringLiteral();
        if (!match(TokenType::EQUAL)) {
            assert(false && "expected equal sign");
        }
        auto var = std::make_unique<AstVariable>(ident);
        auto expr = std::unique_ptr<AstExpression>(parseExpression());
        AstAssignment* assignment =
            new AstAssignment(true, std::move(var), std::move(expr));
        if (!match(TokenType::SEMICOLON)) {
            assert(false && "expected semicolon");
        }
        return assignment;
    } else if (peek().type == TokenType::IDENT) {
        std::string ident = advance().getStringLiteral();

        if (!match(TokenType::EQUAL)) {
            assert(false && "expected equal sign");
        }
        auto var = std::make_unique<AstVariable>(ident);
        auto expr = std::unique_ptr<AstExpression>(parseExpression());
        AstAssignment* assignment =
            new AstAssignment(false, std::move(var), std::move(expr));
        if (!match(TokenType::SEMICOLON)) {
            assert(false && "expected semicolon");
        }
        return assignment;
    } else {
        assert(false && "expected 'var' or identifier");
    }
}

AstExpression* Parser::parseExpression() {
    if (peek().type == TokenType::DOLLAR) {
        assert(false && "not implemented");
    } else if (peek().type == TokenType::RAW) {
        assert(false && "not implemented");
    } else {
        auto expr = parseTerm();

        while (peek().type == TokenType::PLUS ||
               peek().type == TokenType::MINUS) {
            TokenType op = advance().type;
            auto rhs = std::unique_ptr<AstExpression>(parseTerm());
            expr = new AstBinaryExpression(
                op, std::unique_ptr<AstExpression>(expr), std::move(rhs));
        }

        return expr;
    }
}

AstExpression* Parser::parseTerm() {
    auto expr = parseFactor();

    while (peek().type == TokenType::STAR || peek().type == TokenType::SLASH) {
        TokenType op = advance().type;
        auto rhs = std::unique_ptr<AstExpression>(parseFactor());
        expr = new AstBinaryExpression(op, std::unique_ptr<AstExpression>(expr),
                                       std::move(rhs));
    }

    return expr;
}

AstExpression* Parser::parseFactor() {
    Token next = advance();
    if (next.type == TokenType::NUMBER) {
        return new AstNumberLiteral(next.getNumberLiteral());
    } else {
        assert(false && "unimplemented");
    }
}

AstFunction* Parser::parseFunction() {
    if (!match(TokenType::FUNC)) {
        assert(false && "expected 'func'");
    }

    Token next = advance();
    if (next.type != TokenType::IDENT) {
        assert(false && "expected ident");
    }
    std::string name = next.getStringLiteral();
    if (!match(TokenType::LPAREN)) {
        assert(false && "expected '('");
    }

    AstFunction* function = new AstFunction(name);
    if (!match(TokenType::RPAREN)) {
        do {
            Token arg = advance();
            if (arg.type != TokenType::IDENT) {
                assert(false && "expected arguments");
            }
            auto var = std::make_unique<AstVariable>(arg.getStringLiteral());
            function->addArgument(std::move(var));
        } while (match(TokenType::COMMA));

        if (!match(TokenType::RPAREN)) {
            assert(false && "expected ')'");
        }
    }

    if (!match(TokenType::LBRACE)) {
        assert(false && "expected '{'");
    }

    while (hasNext() && peek().type != TokenType::RBRACE) {
        function->addStatement(std::unique_ptr<AstStatement>(parseStatement()));
    }

    if (!match(TokenType::RBRACE)) {
        assert(false && "expected '}'");
    }

    return function;
}

AstStatement* Parser::parseStatement() {
    if (peek().type == TokenType::VAR) {
        return parseAssignment();
    } else {
        assert(false && "unimplemented");
    }
}

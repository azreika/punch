#include "Parser.h"
#include "AstNode.h"
#include "AstStatement.h"
#include "Token.h"

// TODO: error handling

AstProgram* Parser::parseProgram() {
    AstProgram* program = new AstProgram();

    while (hasNext()) {
        Token next = advance();
        if (next.type == TokenType::FUNC) {
            assert(false && "not implemented");
        } else if (next.type == TokenType::VAR) {
            std::string ident = advance().getStringLiteral();
            if (!match(TokenType::EQUAL)) {
                assert(false && "expected equal sign");
            }

            auto var = std::make_unique<AstVariable>(ident);
            auto expr = std::unique_ptr<AstExpression>(parseExpression());
            auto assignment = std::make_unique<AstAssignment>(
                true, std::move(var), std::move(expr));

            program->addAssignment(std::move(assignment));

            if (!match(TokenType::SEMICOLON)) {
                std::cout << getSymbolForTokenType(next.type) << std::endl;
                assert(false && "expected semicolon");
            }
        } else if (next.type == TokenType::IDENT) {
            std::string ident = next.getStringLiteral();

            if (!match(TokenType::EQUAL)) {
                assert(false && "expected equal sign");
            }

            auto var = std::make_unique<AstVariable>(ident);
            auto expr = std::unique_ptr<AstExpression>(parseExpression());
            auto assignment = std::make_unique<AstAssignment>(
                false, std::move(var), std::move(expr));

            program->addAssignment(std::move(assignment));

            if (!match(TokenType::SEMICOLON)) {
                std::cout << getSymbolForTokenType(next.type) << std::endl;
                assert(false && "expected semicolon");
            }
        } else {
            assert(false && "unexpected token");
        }
    }

    return program;
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

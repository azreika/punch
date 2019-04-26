#include "Parser.h"
#include "AstNode.h"
#include "AstStatement.h"
#include "PunchException.h"
#include "Token.h"

AstProgram* Parser::parseProgram() {
    /*  program
     *      : (fundecl | assignment)* END
     */

    AstProgram* program = new AstProgram();

    while (hasNext()) {
        if (peek().type == TokenType::FUNC) {
            // parse function declaration
            auto function = std::unique_ptr<AstFunction>(parseFunction());
            program->addFunction(std::move(function));
        } else if (peek().type == TokenType::VAR ||
                   (peek().type == TokenType::IDENT &&
                    peek(1).type == TokenType::EQUAL)) {
            // parse assignment
            auto assignment = std::unique_ptr<AstAssignment>(parseAssignment());
            program->addAssignment(std::move(assignment));
        } else {
            // neither a function definition nor an assignment - error
            generateError(advance().type,
                          {TokenType::FUNC, TokenType::VAR, TokenType::IDENT});
        }
    }

    return program;
}

AstFunction* Parser::parseFunction() {
    /*  fundecl
     *      : FUNC IDENT LPAREN arglist RPAREN LBRACE (stmt)* RBRACE
     */

    // FUNC
    if (!match(TokenType::FUNC)) {
        generateError(advance().type, {TokenType::FUNC});
    }

    // IDENT
    Token next = advance();
    if (next.type != TokenType::IDENT) {
        generateError(next.type, {TokenType::IDENT});
    }
    std::string name = next.getStringLiteral();

    // LPAREN
    if (!match(TokenType::LPAREN)) {
        generateError(advance().type, {TokenType::LPAREN});
    }

    AstFunction* function = new AstFunction(name);

    // arglist RPAREN
    if (!match(TokenType::RPAREN)) {
        /*
         * arglist
         *      : (IDENT COMMA)* IDENT
         */
        do {
            Token arg = advance();
            if (arg.type != TokenType::IDENT) {
                generateError(advance().type, {TokenType::IDENT});
            }
            auto var = std::make_unique<AstVariable>(arg.getStringLiteral());
            function->addArgument(std::move(var));
        } while (match(TokenType::COMMA));

        if (!match(TokenType::RPAREN)) {
            generateError(advance().type, {TokenType::RPAREN});
        }
    }

    // LBRACE
    if (!match(TokenType::LBRACE)) {
        generateError(advance().type, {TokenType::LBRACE});
    }

    // stmt*
    while (hasNext() && peek().type != TokenType::RBRACE) {
        function->addStatement(std::unique_ptr<AstStatement>(parseStatement()));
    }

    // RBRACE
    if (!match(TokenType::RBRACE)) {
        generateError(advance().type, {TokenType::RBRACE});
    }

    return function;
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
    if (match(TokenType::DOLLAR)) {
        if (!match(TokenType::LPAREN)) {
            assert(false && "expected '('");
        }
        AstRawEnvironment* rawEnv = parseRawEnvironment();
        if (!match(TokenType::RPAREN)) {
            assert(false && "expected ')'");
        }
        return rawEnv;
    } else {
        auto expr = parseTerm();

        while (peek().type == TokenType::PLUS ||
               peek().type == TokenType::MINUS) {
            char op;
            switch (advance().type) {
                case TokenType::PLUS: op = '+'; break;
                case TokenType::MINUS: op = '-'; break;
                case TokenType::STAR: op = '*'; break;
                case TokenType::SLASH: op = '/'; break;
                default: assert(false && "expected binary operator");
            }
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
        char op = advance().type == TokenType::STAR ? '*' : '/';
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
    } else if (next.type == TokenType::IDENT) {
        if (peek().type == TokenType::LPAREN) {
            assert(false && "unimplemented");
        } else {
            return new AstVariable(next.getStringLiteral());
        }
    } else {
        std::cout << next << std::endl;
        assert(false && "unimplemented");
    }
}

AstStatement* Parser::parseStatement() {
    if (peek().type == TokenType::VAR) {
        return parseAssignment();
    } else if (match(TokenType::FOR)) {
        assert(false && "unimplemented");
    } else if (match(TokenType::WHILE)) {
        assert(false && "unimplemented");
    } else if (match(TokenType::IF)) {
        assert(false && "unimplemented");
    } else if (match(TokenType::LBRACE)) {
        assert(false && "unimplemented");
    } else if (match(TokenType::RETURN)) {
        auto expr = std::unique_ptr<AstExpression>(parseExpression());
        AstReturn* result = new AstReturn(std::move(expr));
        if (!match(TokenType::SEMICOLON)) {
            assert(false && "expected ';'");
        }
        return result;
    } else if (match(TokenType::RAW)) {
        if (!match(TokenType::LBRACE)) {
            assert(false && "expected '{'");
        }
        AstRawEnvironment* rawEnv = parseRawEnvironment();
        if (!match(TokenType::RBRACE)) {
            assert(false && "expected '}'");
        }
        return rawEnv;
    } else if (peek().type == TokenType::IDENT &&
               peek(1).type == TokenType::EQUAL) {
        return parseAssignment();
    } else {
        AstExpression* expr = parseExpression();
        if (!match(TokenType::SEMICOLON)) {
            assert(false && "expected ';'");
        }
        return expr;
    }
}

AstRawEnvironment* Parser::parseRawEnvironment() {
    AstRawEnvironment* rawEnv = new AstRawEnvironment();
    while (peek().type == TokenType::RAWEXPR ||
           peek().type == TokenType::DOLLAR) {
        if (peek().type == TokenType::RAWEXPR) {
            std::string expr = advance().getStringLiteral();
            rawEnv->addRawExpression(
                std::make_unique<AstRawBashExpression>(expr));
        } else if (match(TokenType::DOLLAR)) {
            if (!match(TokenType::LBRACKET)) {
                assert(false && "expected '['");
            }
            auto expression = std::unique_ptr<AstExpression>(parseExpression());
            rawEnv->addRawExpression(
                std::make_unique<AstRawPunchExpression>(std::move(expression)));
            if (!match(TokenType::RBRACKET)) {
                assert(false && "expected ']'");
            }
        } else {
            assert(false && "impossible case");
        }
    }
    return rawEnv;
}

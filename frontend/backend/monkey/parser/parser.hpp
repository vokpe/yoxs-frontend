#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>
#include <sstream>
#include "../lexer/lexer.hpp"
#include "../token/token.hpp"
#include "../ast/ast.hpp"

using namespace YOXS_AST;

enum Precedence {
    LOWEST = 1,
    EQUALS, // ==
    LESSGREATER, // > or <
    SUM, // + 
    PRODUCT, // * 
    PREFIX, //-X or !X
    CALL, //myFunction(X)
    INDEX // array[index]
};

extern std::unordered_map<TokenType, Precedence> precedences;

class Parser {
public:
    Parser(Lexer& l);

    std::vector<std::string> Errors() const; 
    std::shared_ptr<Program> ParseProgram();

private:
    Lexer* lexer;
    Token curToken;
    Token peekToken;
    //curToken and peekToken act exactly like the two “pointers” our 
    //lexer has: position and readPosition. But instead of pointing 
    //to a character in the input, they point to the current and the 
    //next token. Both are important: we need to look at the curToken, 
    //which is the current token under examination, to decide what to 
    //do next, and we also need peekToken for this decision if 
    //curToken doesn’t give us enough information.
    std::vector<std::string> errors;

    using prefixParseFn = std::function<std::shared_ptr<Expression>(void)>;
    using infixParseFn = std::function<std::shared_ptr<Expression>(std::shared_ptr<Expression>)>;

    std::unordered_map<TokenType, prefixParseFn> prefixParseFns;
    std::unordered_map<TokenType, infixParseFn> infixParseFns;

    void registerPrefix(TokenType type, prefixParseFn fn);
    void registerInfix(TokenType type, infixParseFn fn);

    void nextToken();
    bool curTokenIs(TokenType t) const;
    bool peekTokenIs(TokenType t) const;
    bool expectPeek(TokenType t);
    void peekError(TokenType t);
    void noPrefixParseFnError(TokenType t);
    int peekPrecedence() const;
    int curPrecedence() const;

    // Parsing functions here...

    std::shared_ptr<Statement> parseStatement();
    std::shared_ptr<LetStatement> parseLetStatement();
    std::shared_ptr<ReturnStatement> parseReturnStatement();
    std::shared_ptr<ExpressionStatement> parseExpressionStatement();

    std::shared_ptr<Expression> parseExpression(Precedence pVal);

    std::shared_ptr<Identifier> parseIdentifier();
    std::shared_ptr<IntegerLiteral> parseIntegerLiteral();
    std::shared_ptr<StringLiteral> parseStringLiteral();

    std::shared_ptr<PrefixExpression> parsePrefixExpression();
    std::shared_ptr<InfixExpression> parseInfixExpression(std::shared_ptr<Expression> left);
    std::shared_ptr<YOXS_AST::Boolean> parseBoolean();
    std::shared_ptr<Expression> parseGroupedExpression();
    std::shared_ptr<IfExpression> parseIfExpression();
    std::shared_ptr<BlockStatement> parseBlockStatement();
    std::shared_ptr<FunctionLiteral> parseFunctionLiteral();
    std::vector<std::shared_ptr<Identifier>> parseFunctionParameters();     
    std::shared_ptr<CallExpression> parseCallExpression(std::shared_ptr<Expression> function);
    std::vector<std::shared_ptr<Expression>> parseExpressionList(const TokenType& end);
    std::shared_ptr<ArrayLiteral> parseArrayLiteral();
    std::shared_ptr<IndexExpression> parseIndexExpression(std::shared_ptr<Expression> left);
    std::shared_ptr<HashLiteral> parseHashLiteral();
    

};

#endif // PARSER_H

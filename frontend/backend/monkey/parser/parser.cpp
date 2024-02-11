#include "parser.hpp"

std::unordered_map<TokenType, Precedence> precedences = {
    { TokenType::EQ, EQUALS },
    { TokenType::NOT_EQ, EQUALS },
    { TokenType::LT, LESSGREATER },
    { TokenType::GT, LESSGREATER},
    { TokenType::PLUS, SUM},
    { TokenType::MINUS, SUM},
    { TokenType::SLASH, PRODUCT},
    { TokenType::ASTERISK, PRODUCT},
    { TokenType::LPAREN, CALL},
    { TokenType::LBRACKET, INDEX}

};

Parser::Parser(Lexer& l) : lexer(&l) {
    // Initialize errors
    errors = std::vector<std::string>();


    // Register prefix functions using lambda functions for explicit casting
    registerPrefix(TokenType::IDENT, [this]() -> std::shared_ptr<Expression> {
        return this->parseIdentifier();
    });
    registerPrefix(TokenType::INT, [this]() -> std::shared_ptr<Expression> {
        return this->parseIntegerLiteral();
    });
    registerPrefix(TokenType::STRING, [this]() -> std::shared_ptr<Expression> {
        return this->parseStringLiteral();
    });
    registerPrefix(TokenType::BANG, [this]() -> std::shared_ptr<Expression> {
        return this->parsePrefixExpression();
    });
    registerPrefix(TokenType::MINUS, [this]() -> std::shared_ptr<Expression> {
        return this->parsePrefixExpression();
    });
    registerPrefix(TokenType::TRUE, [this]() -> std::shared_ptr<Expression> {
        return this->parseBoolean();
    });
    registerPrefix(TokenType::FALSE, [this]() -> std::shared_ptr<Expression> {
        return this->parseBoolean();
    });
    registerPrefix(TokenType::LPAREN, [this]() -> std::shared_ptr<Expression> {
        return this->parseGroupedExpression();
    });
    registerPrefix(TokenType::IF, [this]() -> std::shared_ptr<Expression> {
        return this->parseIfExpression();
    });
    registerPrefix(TokenType::FUNCTION, [this]() -> std::shared_ptr<Expression> {
        return this->parseFunctionLiteral();
    });
    registerPrefix(TokenType::LBRACKET, [this]() -> std::shared_ptr<Expression> {
        return this->parseArrayLiteral();
    });
    registerPrefix(TokenType::LBRACE, [this]() -> std::shared_ptr<Expression> {
        return this->parseHashLiteral();
    });


    // Register infix functions using lambda functions for explicit casting
    registerInfix(TokenType::PLUS, [this](std::shared_ptr<Expression> left) -> std::shared_ptr<Expression> {
        return this->parseInfixExpression(left);
    });
    registerInfix(TokenType::MINUS, [this](std::shared_ptr<Expression> left) -> std::shared_ptr<Expression> {
        return this->parseInfixExpression(left);
    });
    registerInfix(TokenType::SLASH, [this](std::shared_ptr<Expression> left) {
        return this->parseInfixExpression(left);
    });
    registerInfix(TokenType::ASTERISK, [this](std::shared_ptr<Expression> left) {
        return this->parseInfixExpression(left);
    });
    registerInfix(TokenType::EQ, [this](std::shared_ptr<Expression> left) {
        return this->parseInfixExpression(left);
    });
    registerInfix(TokenType::NOT_EQ, [this](std::shared_ptr<Expression> left) {
        return this->parseInfixExpression(left);
    });
    registerInfix(TokenType::LT, [this](std::shared_ptr<Expression> left) {
        return this->parseInfixExpression(left);
    });
    registerInfix(TokenType::GT, [this](std::shared_ptr<Expression> left) {
        return this->parseInfixExpression(left);
    });
    registerInfix(TokenType::LPAREN, [this](std::shared_ptr<Expression> function) {
        return this->parseCallExpression(function);
    });
    registerInfix(TokenType::LBRACKET, [this](std::shared_ptr<Expression> idx) {
        return this->parseIndexExpression(idx);
    });

    // Read two tokens, so curToken and peekToken are both set
    nextToken();
    nextToken();
}


void Parser::registerPrefix(TokenType type, prefixParseFn fn) {
    prefixParseFns[type] = fn;
}

void Parser::registerInfix(TokenType type, infixParseFn fn) {
    infixParseFns[type] = fn;
}

void Parser::nextToken() {
    curToken = peekToken;
    peekToken = lexer->NextToken();
}

bool Parser::curTokenIs(TokenType t) const {
    return curToken.Type == t;
}

bool Parser::peekTokenIs(TokenType t) const {
    return peekToken.Type == t;
}

bool Parser::expectPeek(TokenType t) {
    if(peekTokenIs(t)) {
        nextToken();
        return true;
    } else{
        peekError(t);
        return false;
    }
}

void Parser::peekError(TokenType t) {
    std::ostringstream oss; 
    oss << "expected next token to be " << t << ", got " << peekToken.Type << " instead";
    errors.push_back(oss.str());
}

void Parser::noPrefixParseFnError(TokenType t) {
    std::ostringstream oss;
    oss << "no prefix parse function for " << t << " found";
    errors.push_back(oss.str());
}


int Parser::peekPrecedence() const {
    auto it = precedences.find(peekToken.Type);
    if (it != precedences.end()) {
        return it->second;
    }
    return Precedence::LOWEST;
}

int Parser::curPrecedence() const {
    auto it = precedences.find(curToken.Type);
    if (it != precedences.end()) {
        return it->second;
    }
    return Precedence::LOWEST;
}

// Parsing functions here...

std::shared_ptr<Program> Parser::ParseProgram() {
    auto program = std::make_shared<Program>();
    program->Statements = std::vector<std::shared_ptr<Statement>>();

    while(!curTokenIs(TokenType::EOF_TOKEN)) {
        auto stmt = parseStatement();
        if(stmt) program->Statements.push_back(stmt);
        nextToken();
    }

    return program;
}

std::shared_ptr<Statement> Parser::parseStatement(){
    switch (curToken.Type)
    {
    case TokenType::LET:
        return parseLetStatement();
        break;
    
    case TokenType::RETURN:
        return parseReturnStatement();
        break;
    
    default: //expression statement
        return parseExpressionStatement();
        break;
    }
}

std::shared_ptr<LetStatement> Parser::parseLetStatement() {
    auto stmt = std::make_shared<LetStatement>();
    stmt->token = curToken;

    if (!expectPeek(TokenType::IDENT)) {
        return nullptr;
    }

    stmt->Name = std::make_shared<Identifier> (curToken, curToken.Literal); 

    if(!expectPeek(TokenType::ASSIGN)) {
        return nullptr;
    }

    nextToken();
    stmt->Value = parseExpression(Precedence::LOWEST);

    if(peekTokenIs(TokenType::SEMICOLON)){
        nextToken();
    }

    return stmt;
}

std::shared_ptr<ReturnStatement> Parser::parseReturnStatement() {
    auto stmt = std::make_shared<ReturnStatement>();
    stmt->token = curToken;

    nextToken();
    stmt->ReturnValue = parseExpression(Precedence::LOWEST);

    if (peekTokenIs(TokenType::SEMICOLON)) {
        nextToken();
    }

    return stmt;
}

std::shared_ptr<ExpressionStatement> Parser::parseExpressionStatement(){
    auto stmt = std::make_shared<ExpressionStatement>();
    stmt->expr = parseExpression(Precedence::LOWEST); // check if valid

    if (peekTokenIs(TokenType::SEMICOLON)){
        nextToken();
    }

    return stmt;
}

std::shared_ptr<Expression> Parser::parseExpression(Precedence pVal){
    auto prefixIt = prefixParseFns.find(curToken.Type);
    if (prefixIt == prefixParseFns.end()) {
        noPrefixParseFnError(curToken.Type);
        return nullptr;
    }
    auto prefix = prefixIt->second;
    std::shared_ptr<Expression> leftExp = (prefix)();
    //here we are calling prefix like a function and the parentheses after prefix 
    //are invoking the callable object. if prefix is a lambda or std::function
    //wrapping a lambda it will invoke the lambdas code. 
    //The result is stored in leftExp

    while(!peekTokenIs(TokenType::SEMICOLON) && pVal < peekPrecedence()) {
        auto infixIt = infixParseFns.find(peekToken.Type);
        if (infixIt == infixParseFns.end()) return leftExp;
        nextToken();

        auto infix = infixIt->second;
        leftExp = (infix)(leftExp);    }

    return leftExp;

}

std::shared_ptr<Identifier>  Parser::parseIdentifier(){
    return std::make_shared<Identifier> (curToken, curToken.Literal);
}

std::shared_ptr<IntegerLiteral>  Parser::parseIntegerLiteral(){
    auto lit = std::make_shared<IntegerLiteral>(curToken);

    try {
        //stoi can throw an exception if conversion fails
        lit->Value = std::stoll(curToken.Literal);
    } catch (const std::exception& e){
        std::string msg = "could not parse \"" + curToken.Literal + "\" as integer";
        errors.push_back(msg);
        return nullptr;
    }

    return lit;
}

std::shared_ptr<StringLiteral> Parser::parseStringLiteral() {
    return std::make_shared<StringLiteral>(curToken, curToken.Literal);
}

std::shared_ptr<PrefixExpression>  Parser::parsePrefixExpression(){
    auto expression = std::make_shared<PrefixExpression>(curToken, curToken.Literal);

    nextToken();

    expression->Right = parseExpression(Precedence::PREFIX);

    return expression;

}

std::shared_ptr<InfixExpression> Parser::parseInfixExpression (std::shared_ptr<Expression> left){
    auto expression = std::make_shared<InfixExpression>(curToken, curToken.Literal, left);
    auto precedence = curPrecedence();

    nextToken();
    expression->Right = parseExpression(static_cast<Precedence>(precedence));

    return expression;
}

std::shared_ptr<YOXS_AST::Boolean> Parser::parseBoolean(){
    return std::make_shared<YOXS_AST::Boolean> (curToken, curTokenIs(TokenType::TRUE));
}

std::shared_ptr<Expression> Parser::parseGroupedExpression(){
    nextToken();

    auto exp = parseExpression(Precedence::LOWEST);

    if(!expectPeek(TokenType::RPAREN)) {
        return nullptr;
    }

    return exp;
}

std::shared_ptr<IfExpression>  Parser::parseIfExpression() {
    auto expression = std::make_shared<IfExpression>(curToken);

    if(!expectPeek(TokenType::LPAREN)) {
        return nullptr;
    }

    nextToken();
    expression->Condition = parseExpression(Precedence::LOWEST);

    if(!expectPeek(TokenType::RPAREN)) {
        return nullptr;
    }

    if(!expectPeek(TokenType::LBRACE)){
        return nullptr;
    }

    expression->Consequence = parseBlockStatement();

    if(peekTokenIs(TokenType::ELSE)) {
        nextToken();

        if(!expectPeek(TokenType::LBRACE)) {
            return nullptr;
        }

        expression->Alternative = parseBlockStatement();
    }

    return expression;
}

std::shared_ptr<BlockStatement> Parser::parseBlockStatement(){
    auto block = std::make_shared<BlockStatement>(curToken);

    nextToken();

    while (!curTokenIs(TokenType::RBRACE) && !curTokenIs(TokenType::EOF_TOKEN)) {
        auto stmt = parseStatement();
        if (stmt) {
            block->Statements.push_back(stmt);
        }
        nextToken();
    }

    return block;
}

std::shared_ptr<FunctionLiteral> Parser::parseFunctionLiteral(){
    auto lit = std::make_shared<FunctionLiteral>(curToken);

    if(!expectPeek(TokenType::LPAREN)) {
        return nullptr;
    }

    lit->Parameters = parseFunctionParameters();

    if(!expectPeek(TokenType::LBRACE)) {
        return nullptr;
    }

    lit->Body = parseBlockStatement();

    return lit;
}
std::vector<std::shared_ptr<Identifier>>  Parser::parseFunctionParameters() {
    std::vector<std::shared_ptr<Identifier>> identifiers;

    if(peekTokenIs(TokenType::RPAREN)) {
        nextToken();
        return identifiers;
    }

    nextToken();

    auto ident = std::make_shared<Identifier>(curToken, curToken.Literal);
    identifiers.push_back(ident);

    while (peekTokenIs(TokenType::COMMA)){
        nextToken();  // Consume the COMMA
        nextToken();  // Move to the next token after the COMMA
        ident = std::make_shared<Identifier>(curToken, curToken.Literal);
        identifiers.push_back(ident);
    }

    if (!expectPeek(TokenType::RPAREN)) {
        // in C++ we can't return nullptr for a vector.
        // For now, I'll clear the vector and return it, indicating an error.
        identifiers.clear();
        return identifiers;
    }

    return identifiers;


}

std::shared_ptr<CallExpression> Parser::parseCallExpression(std::shared_ptr<Expression> function){
    auto exp = std::make_shared<CallExpression>(curToken, function);
    exp->Arguments = parseExpressionList(TokenType::RPAREN);
    return exp;
}

std::vector<std::shared_ptr<Expression>> Parser::parseExpressionList(const TokenType& end){
    std::vector<std::shared_ptr<Expression>> list;
    if(peekTokenIs(end)) {
        nextToken();
        return list;
    }
    nextToken();
    list.push_back(parseExpression(Precedence::LOWEST));
    while(peekTokenIs(TokenType::COMMA)){  //todo check this
        nextToken(); // consume the COMMA
        nextToken(); // move to the next token after the COMMA
        list.push_back(parseExpression(Precedence::LOWEST));
    }

    if(!expectPeek(end)){
        // Return an empty vector to indicate an error or unexpected input
        list.clear();
        return list;
    }

    return list;
}

std::shared_ptr<ArrayLiteral> Parser::parseArrayLiteral(){
    auto array = std::make_shared<ArrayLiteral>(curToken);

    array->Elements = parseExpressionList(TokenType::RBRACKET);

    return array;
}

std::shared_ptr<IndexExpression> Parser::parseIndexExpression(std::shared_ptr<Expression> left){
    auto exp = std::make_shared<IndexExpression>(curToken, left);
    nextToken();
    exp->Index = parseExpression(Precedence::LOWEST);

    if(!expectPeek(TokenType::RBRACKET)) return nullptr;

    return exp;
}

std::shared_ptr<HashLiteral> Parser::parseHashLiteral(){
    auto hash = std::make_shared<HashLiteral>(curToken);
    while(!peekTokenIs(TokenType::RBRACE)) {
        nextToken();
        auto key = parseExpression(Precedence::LOWEST);

        if(!expectPeek(TokenType::COLON)) return nullptr;

        nextToken();
        auto value = parseExpression(Precedence::LOWEST);
        hash->Pairs[key] = value;

        if(!peekTokenIs(TokenType::RBRACE) && !expectPeek(TokenType::COMMA)) return nullptr;
    }

    if(!expectPeek(TokenType::RBRACE)) return nullptr;

    return hash;
}

std::vector<std::string> Parser::Errors() const {
    return errors; 
}

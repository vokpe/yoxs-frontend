#include "ast.hpp"
#include <iostream>

//AST Test: This tests the construction of an Abstract Syntax Tree (AST) and its string representation.
using namespace YOXS_AST;

void TestString() {
    Program program;

    // Setting up the LetStatement
    auto letStatement = std::make_shared<LetStatement>();
    letStatement->token = Token{TokenType::LET, "let"};

    // Setting up the Name Identifier
    auto name = std::make_shared<Identifier>();
    name->token = Token{TokenType::IDENT, "myVar"};
    letStatement->Name = std::move(name);

    // Setting up the Value Identifier
    auto value = std::make_shared<Identifier>();
    value->token = Token{TokenType::IDENT, "anotherVar"};
    letStatement->Value = std::move(value);

    program.Statements.push_back(std::move(letStatement));

    if (program.String() != "let myVar = anotherVar;") {
        std::cerr << "program.String() is wrong. got: " << program.String() << std::endl;
        exit(1);
    } else {
        std::cout << "TestString passed!" << std::endl;
    }
}

void TestStringLiteral() {
    StringLiteral strLiteral(Token{TokenType::STRING, "\"Hello World\""});
    if (strLiteral.String() != "\"Hello World\"") {
        std::cerr << "StringLiteral.String() is wrong. got: " << strLiteral.String() << std::endl;
        exit(1);
    } else {
        std::cout << "TestStringLiteral passed!" << std::endl;
    }
}

void TestArrayLiteral() {
    ArrayLiteral arrLiteral(Token{TokenType::LBRACKET, "["});
    arrLiteral.Elements.push_back(std::make_shared<IntegerLiteral>(Token{TokenType::INT, "1"}, 1));
    arrLiteral.Elements.push_back(std::make_shared<IntegerLiteral>(Token{TokenType::INT, "2"}, 2));

    if (arrLiteral.String() != "[1, 2]") {
        std::cerr << "ArrayLiteral.String() is wrong. got: " << arrLiteral.String() << std::endl;
        exit(1);
    } else {
        std::cout << "TestArrayLiteral passed!" << std::endl;
    }
}

void TestIndexExpression() {
    auto leftExp = std::make_shared<Identifier>(Token{TokenType::IDENT, "myArray"}, "myArray");
    auto indexExp = std::make_shared<IntegerLiteral>(Token{TokenType::INT, "0"});
    indexExp->Value = 0;
    IndexExpression indexExpression(Token{TokenType::LBRACKET, "["}, leftExp);
    indexExpression.Index = indexExp;

    if (indexExpression.String() != "(myArray[0])") { // Adjusted expected string based on your `String()` method
        std::cerr << "IndexExpression.String() is wrong. got: " << indexExpression.String() << std::endl;
        exit(1);
    } else {
        std::cout << "TestIndexExpression passed!" << std::endl;
    }
}

void TestHashLiteral() {
    HashLiteral hashLiteral(Token{TokenType::LBRACE, "{"});
    auto key = std::make_shared<StringLiteral>(Token{TokenType::STRING, "\"key\""});
    auto value = std::make_shared<StringLiteral>(Token{TokenType::STRING, "\"value\""});
    hashLiteral.Pairs[key] = value;

    if (hashLiteral.String() != "{\"key\":\"value\"}") {
        std::cerr << "HashLiteral.String() is wrong. got: " << hashLiteral.String() << std::endl;
        exit(1);
    } else {
        std::cout << "TestHashLiteral passed!" << std::endl;
    }
}

int main() {
    TestString();
    TestStringLiteral();
    TestArrayLiteral();
    TestIndexExpression();
    TestHashLiteral();
    std::cout << "all ast_test.cpp tests passed" << std::endl;
    return 0;
}

//g++ -std=c++17 -Isrc -o ast_test src/monkey/ast/ast.cpp src/monkey/token/token.cpp src/monkey/ast/ast_test.cpp && ./ast_test

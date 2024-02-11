#include "token.hpp"
#include <iostream>
#include <cassert>

//Token Test: This tests the token construction and the function that looks up identifiers to determine if they are keywords or general identifiers.

int main() {
    // Test 1: Token construction
    Token t1(TokenType::INT, "12345");
    assert(t1.Type == TokenType::INT);
    assert(t1.Literal == "12345");
    std::cout << "Token construction test passed!" << std::endl;

    // Test 2: LookupIdent for keywords
    assert(LookupIdent("fn") == TokenType::FUNCTION);
    assert(LookupIdent("let") == TokenType::LET);
    assert(LookupIdent("true") == TokenType::TRUE);
    std::cout << "LookupIdent for keywords test passed!" << std::endl;

    // Test 3: LookupIdent for identifiers
    assert(LookupIdent("foobar") == TokenType::IDENT);
    assert(LookupIdent("x") == TokenType::IDENT);
    std::cout << "LookupIdent for identifiers test passed!" << std::endl;

    std::cout << "All token_test.cpp tests passed!" << std::endl;
    return 0;
}
//g++ -std=c++17 -Isrc -o token_test src/monkey/token/token_test.cpp src/monkey/token/token.cpp && ./token_test

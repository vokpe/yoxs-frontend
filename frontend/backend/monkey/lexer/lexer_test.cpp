
#include "lexer.hpp"
#include <iostream>
#include <string>
#include <vector>

//Lexer Test: This tests the lexer's functionality, ensuring it tokenizes a given input string correctly.

int main() {
    std::string input = R"(
let five = 5;
let ten = 10;

let add = fn(x, y) {
  x + y;
};

let result = add(five, ten);
!-/*5;
5 < 10 > 5;

if (5 < 10) {
	return true;
} else {
	return false;
}

10 == 10;
10 != 9;
"foobar"
"foo bar"
[1, 2];
{"foo": "bar"}
)";

    struct Test {
        TokenType expectedType;
        std::string expectedLiteral;
    };

    std::vector<Test> tests = {
        {TokenType::LET, "let"},
        {TokenType::IDENT, "five"},
        {TokenType::ASSIGN, "="},
        {TokenType::INT, "5"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::LET, "let"},
        {TokenType::IDENT, "ten"},
        {TokenType::ASSIGN, "="},
        {TokenType::INT, "10"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::LET, "let"},
        {TokenType::IDENT, "add"},
        {TokenType::ASSIGN, "="},
        {TokenType::FUNCTION, "fn"},
        {TokenType::LPAREN, "("},
        {TokenType::IDENT, "x"},
        {TokenType::COMMA, ","},
        {TokenType::IDENT, "y"},
        {TokenType::RPAREN, ")"},
        {TokenType::LBRACE, "{"},
        {TokenType::IDENT, "x"},
        {TokenType::PLUS, "+"},
        {TokenType::IDENT, "y"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::RBRACE, "}"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::LET, "let"},
        {TokenType::IDENT, "result"},
        {TokenType::ASSIGN, "="},
        {TokenType::IDENT, "add"},
        {TokenType::LPAREN, "("},
        {TokenType::IDENT, "five"},
        {TokenType::COMMA, ","},
        {TokenType::IDENT, "ten"},
        {TokenType::RPAREN, ")"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::BANG, "!"},
        {TokenType::MINUS, "-"},
        {TokenType::SLASH, "/"},
        {TokenType::ASTERISK, "*"},
        {TokenType::INT, "5"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::INT, "5"},
        {TokenType::LT, "<"},
        {TokenType::INT, "10"},
        {TokenType::GT, ">"},
        {TokenType::INT, "5"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::IF, "if"},
        {TokenType::LPAREN, "("},
        {TokenType::INT, "5"},
        {TokenType::LT, "<"},
        {TokenType::INT, "10"},
        {TokenType::RPAREN, ")"},
        {TokenType::LBRACE, "{"},
        {TokenType::RETURN, "return"},
        {TokenType::TRUE, "true"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::RBRACE, "}"},
        {TokenType::ELSE, "else"},
        {TokenType::LBRACE, "{"},
        {TokenType::RETURN, "return"},
        {TokenType::FALSE, "false"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::RBRACE, "}"},
        {TokenType::INT, "10"},
        {TokenType::EQ, "=="},
        {TokenType::INT, "10"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::INT, "10"},
        {TokenType::NOT_EQ, "!="},
        {TokenType::INT, "9"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::STRING, "foobar"},
		{TokenType::STRING, "foo bar"},
		{TokenType::LBRACKET, "["},
		{TokenType::INT, "1"},
		{TokenType::COMMA, ","},
		{TokenType::INT, "2"},
		{TokenType::RBRACKET, "]"},
		{TokenType::SEMICOLON, ";"},
		{TokenType::LBRACE, "{"},
		{TokenType::STRING, "foo"},
		{TokenType::COLON, ":"},
		{TokenType::STRING, "bar"},
		{TokenType::RBRACE, "}"},
        {TokenType::EOF_TOKEN, ""}
    };

    Lexer l(input);

    for (size_t i = 0; i < tests.size(); ++i) {
        Token tok = l.NextToken();

        if (tok.Type != tests[i].expectedType) {
            std::cerr << "Test[" << i << "] - TokenType wrong. Expected=" 
                      << static_cast<int>(tests[i].expectedType) 
                      << ", Got=" << static_cast<int>(tok.Type) << std::endl;
            return 1; // Exit with error
        }

        if (tok.Literal != tests[i].expectedLiteral) {
            std::cerr << "Test[" << i << "] - Literal wrong. Expected=" 
                      << tests[i].expectedLiteral << ", Got=" 
                      << tok.Literal << std::endl;
            return 1; // Exit with error
        }
    }

    std::cout << "All lexer_test.cpp tests passed!" << std::endl;

    return 0;
}


//g++ -std=c++17 -Isrc -o lexer_test src/monkey/lexer/lexer.cpp src/monkey/token/token.cpp src/monkey/lexer/lexer_test.cpp && ./lexer_test
//in cpp code file
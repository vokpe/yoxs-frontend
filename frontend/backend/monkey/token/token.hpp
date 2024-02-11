#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include <unordered_map>
#include <iostream>

enum class TokenType {
    ILLEGAL,
    EOF_TOKEN,  // EOF is a macro in C++, so renaming to avoid conflicts

    // Identifiers + literals
    IDENT,  // add, foobar, x, y, ...
    INT,    // 1343456
    STRING, // "foobar"

    // Operators
    ASSIGN, // =
    PLUS,
    MINUS,
    BANG, // !
    ASTERISK, //* 
    SLASH,

    LT,
    GT,

    EQ,
    NOT_EQ,

    // Delimiters
    COMMA,
    SEMICOLON,
    COLON, 

    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    LBRACKET,
    RBRACKET,

    // Keywords
    FUNCTION, //fn
    LET,
    TRUE,
    FALSE,
    IF,
    ELSE,
    RETURN
};

class Token {
public:
    TokenType Type;
    std::string Literal;
    Token() = default;
    Token(TokenType type, const std::string& literal);
};

TokenType LookupIdent(const std::string& ident);

std::string TokenTypeToString(TokenType type);

std::ostream& operator<<(std::ostream& os, const Token& token);

std::ostream& operator<<(std::ostream& os, const TokenType& tokenType);

#endif // TOKEN_H
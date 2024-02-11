#ifndef LEXER_H
#define LEXER_H

#include <string>
#include "../token/token.hpp"

class Lexer {
private:
    std::string input;
    std::string::size_type position;         // current position in input (points to current char)
    std::string::size_type readPosition;     // current reading position in input (after current char)
    char ch;              // current char under examination

    void readChar();
    char peekChar() const;
    std::string readIdentifier();
    std::string readNumber();
    std::string readString();
    void skipWhitespace();
    static bool isLetter(char ch);
    static bool isDigit(char ch);
    static Token newToken(TokenType tokenType, char ch);

public:
    Lexer(const std::string& input);
    Token NextToken();
};

#endif // LEXER_H

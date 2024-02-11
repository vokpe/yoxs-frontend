#include "lexer.hpp"

Lexer::Lexer(const std::string& input) : input(input), position(0), readPosition(0), ch(0) {
    readChar();
}

void Lexer::readChar() {
    if (readPosition >= input.size()) {
        ch = 0;
    } else {
        ch = input[readPosition];
    }
    position = readPosition;
    readPosition++;
}

char Lexer::peekChar() const {
    if (readPosition >= input.size()) {
        return 0;
    }
    return input[readPosition];
}

std::string Lexer::readIdentifier() {
    int startPosition = position;
    while (isLetter(ch)) {
        readChar();
    }
    return input.substr(startPosition, position - startPosition);
}

std::string Lexer::readNumber() {
    int startPosition = position;
    while (isDigit(ch)) {
        readChar();
    }
    return input.substr(startPosition, position - startPosition);
}

std::string Lexer::readString(){
    int startPosition = position + 1;
    do {
        readChar();
    }
    while (ch != '"' && ch != 0);
    return input.substr(startPosition, position - startPosition);
}

void Lexer::skipWhitespace() {
    while (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') {
        readChar();
    }
}

bool Lexer::isLetter(char ch) {
    return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z') || ch == '_';
}

bool Lexer::isDigit(char ch) {
    return '0' <= ch && ch <= '9';
}

Token Lexer::newToken(TokenType tokenType, char ch) {
    return Token(tokenType, std::string(1, ch));
}

Token Lexer::NextToken() {
    skipWhitespace();

    Token tok;

    switch (ch) {
        case '=':
            if (peekChar() == '=') {
                char currCh = ch;
                readChar();
                std::string literal = std::string(1, currCh) + ch;
                tok = Token(TokenType::EQ, literal);
            } else {
                tok = newToken(TokenType::ASSIGN, ch);
            }
            break;
        case '+':
            tok = newToken(TokenType::PLUS, ch);
            break;
        case '-':
            tok = newToken(TokenType::MINUS, ch);
            break;
        case '!':
            if (peekChar() == '=') {
                char currCh = ch;
                readChar();
                std::string literal = std::string(1, currCh) + ch;
                tok = Token(TokenType::NOT_EQ, literal);
            } else {
                tok = newToken(TokenType::BANG, ch); // (!true)
            }
            break;
        case '/':
            tok = newToken(TokenType::SLASH, ch);
            break;
        case '*':
            tok = newToken(TokenType::ASTERISK, ch);
            break;
        case '<':
            tok = newToken(TokenType::LT, ch);
            break;
        case '>':
            tok = newToken(TokenType::GT, ch);
            break;
        case ';':
            tok = newToken(TokenType::SEMICOLON, ch);
            break;
        case ':':
            tok = newToken(TokenType::COLON, ch);
            break;
        case ',':
            tok = newToken(TokenType::COMMA, ch);
            break;
        case '{':
            tok = newToken(TokenType::LBRACE, ch);
            break;
        case '}':
            tok = newToken(TokenType::RBRACE, ch);
            break;
        case '(':
            tok = newToken(TokenType::LPAREN, ch);
            break;
        case ')':
            tok = newToken(TokenType::RPAREN, ch);
            break;
        case '[':
            tok = newToken(TokenType::LBRACKET, ch);
            break;
        case ']':
            tok = newToken(TokenType::RBRACKET, ch);
            break;
        case '"':
            tok.Type = TokenType::STRING;
            tok.Literal = readString();
            break;
        case 0:
            tok = Token(TokenType::EOF_TOKEN, "");
            break;
        default:
            if (isLetter(ch)) {
                std::string identifier = readIdentifier();
                tok = Token(LookupIdent(identifier), identifier);
                return tok;  // Return here because readIdentifier advances the characters
            } else if (isDigit(ch)) {
                std::string num = readNumber();
                tok = Token(TokenType::INT, num);
                return tok;  // Return here because readNumber advances the characters
            } else {
                tok = newToken(TokenType::ILLEGAL, ch);
            }
            break;
    }

    readChar();
    return tok;
}

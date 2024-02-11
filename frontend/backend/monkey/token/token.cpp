#include "token.hpp"

Token::Token(TokenType type, const std::string& literal) : Type(type), Literal(literal) {}

static std::unordered_map<std::string, TokenType> keywords = {
    {"fn", TokenType::FUNCTION},
    {"let", TokenType::LET},
    {"true", TokenType::TRUE},
    {"false", TokenType::FALSE},
    {"if", TokenType::IF},
    {"else", TokenType::ELSE},
    {"return", TokenType::RETURN}
};

TokenType LookupIdent(const std::string& ident) {
    auto it = keywords.find(ident);
    if (it != keywords.end()) {
        return it->second;
    }
    return TokenType::IDENT;
}

std::string TokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::ILLEGAL:      return "ILLEGAL";
        case TokenType::EOF_TOKEN:    return "EOF";

        case TokenType::IDENT:        return "IDENT";
        case TokenType::INT:          return "INT";
        case TokenType::STRING:       return "STRING";

        case TokenType::ASSIGN:       return "=";
        case TokenType::PLUS:         return "+";
        case TokenType::MINUS:        return "-";
        case TokenType::BANG:         return "!";
        case TokenType::ASTERISK:     return "*";
        case TokenType::SLASH:        return "/";

        case TokenType::LT:           return "<";
        case TokenType::GT:           return ">";

        case TokenType::EQ:           return "==";
        case TokenType::NOT_EQ:       return "!=";

        case TokenType::COMMA:        return ",";
        case TokenType::SEMICOLON:    return ";";
        case TokenType::COLON:        return ":";

        case TokenType::LPAREN:       return "(";
        case TokenType::RPAREN:       return ")";
        case TokenType::LBRACE:       return "{";
        case TokenType::RBRACE:       return "}";
        case TokenType::LBRACKET:     return "[";
        case TokenType::RBRACKET:     return "]";

        case TokenType::FUNCTION:     return "FUNCTION";
        case TokenType::LET:          return "LET";
        case TokenType::TRUE:         return "TRUE";
        case TokenType::FALSE:        return "FALSE";
        case TokenType::IF:           return "IF";
        case TokenType::ELSE:         return "ELSE";
        case TokenType::RETURN:       return "RETURN";
        default:                      return "UNKNOWN";
    }
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << TokenTypeToString(token.Type);
    return os;
}

std::ostream& operator<<(std::ostream& os, const TokenType& tokenType) {
    os << TokenTypeToString(tokenType);
    return os;
}

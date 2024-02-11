#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include <iterator>
#include <map>
#include "../token/token.hpp"

namespace YOXS_AST {
// Forward declarations of all the classes we're going to use.
class Statement; 
class Expression;
class Identifier;

// Node represents every node in the abstract syntax tree
class Node {
public:
    virtual ~Node() = default; 
    virtual std::string TokenLiteral() const = 0;
    virtual std::string String() const = 0;
};

// All nodes that can be used as statements implement this interface
class Statement : public Node {
public: 
    virtual void statementNode() = 0;
};

// All nodes that can be used as expressions implement this interface
class Expression : public Node {
public: 
    virtual void expressionNode() = 0;
};

// The root node of every AST our parser produces
class Program : public Node {
public:
    std::vector<std::shared_ptr<Statement>> Statements;
    std::string TokenLiteral() const override;
    std::string String() const override;
};

// AST node for let statements
class LetStatement : public Statement {
public:
    Token token; // The 'let' token
    std::shared_ptr<Identifier> Name;
    std::shared_ptr<Expression> Value;

    std::string TokenLiteral() const override;
    std::string String() const override;
    void statementNode() override {}

};

class ReturnStatement : public Statement {
public:
    Token token; // the 'return' token
    std::shared_ptr<Expression> ReturnValue;

    std::string TokenLiteral() const override;
    std::string String() const override;
    void statementNode() override {}
};

class ExpressionStatement : public Statement {
public:
    Token token; // the first token of the expression
    std::shared_ptr<Expression> expr;

    std::string TokenLiteral() const override;
    std::string String() const override;
    void statementNode() override {}
};

class BlockStatement : public Statement {
public:
    BlockStatement(const Token& t);
    Token token; // the '{' token
    std::vector<std::shared_ptr<Statement>> Statements;

    std::string TokenLiteral() const override;
    std::string String() const override;
    void statementNode() override {}
};

class Identifier : public Expression {
public:
    Identifier() = default;
    Identifier(const Token& t, const std::string& v);

    Token token; // The IDENT token
    std::string Value() const;
    std::string TokenLiteral() const override;
    std::string String() const override;
    void expressionNode() override {}

};

class Boolean : public Expression {
public:
    Boolean(const Token& t, const bool& v);
    Token token;
    bool Value;

    std::string TokenLiteral() const override;
    std::string String() const override;
    void expressionNode() override {}
};

class IntegerLiteral : public Expression {
public: 
    Token token;
    int64_t Value;
    IntegerLiteral(const Token& t) : token(t) {}
    IntegerLiteral(const Token& t, int64_t value) : token(t), Value(value) {}
    std::string TokenLiteral() const override;
    std::string String() const override;
    void expressionNode() override {}
};

class PrefixExpression : public Expression {
public:
    PrefixExpression(const Token& t, const std::string& v);

    Token token; // The prefix token, e.g. !
    std::string Operator;
    std::shared_ptr<Expression> Right;

    std::string TokenLiteral() const override;
    std::string String() const override;
    void expressionNode() override {}
};

class InfixExpression : public Expression {
public:

    InfixExpression(const Token& tok, const std::string& op, std::shared_ptr<Expression> leftExp);
    Token token; // The operator token, e.g. +
    std::shared_ptr<Expression> Left;
    std::string Operator;
    std::shared_ptr<Expression> Right;

    std::string TokenLiteral() const override;
    std::string String() const override;
    void expressionNode() override {}
};

class IfExpression : public Expression {
public:
    IfExpression(const Token& t);
    Token token; // The 'if' token
    std::shared_ptr<Expression> Condition;
    std::shared_ptr<BlockStatement> Consequence;
    std::shared_ptr<BlockStatement> Alternative;

    std::string TokenLiteral() const override;
    std::string String() const override;
    void expressionNode() override {}
};

class FunctionLiteral : public Expression {
public:
    FunctionLiteral(const Token& t);
    Token token; // The 'fn' token
    std::vector<std::shared_ptr<Identifier>> Parameters;
    std::shared_ptr<BlockStatement> Body;

    std::string TokenLiteral() const override;
    std::string String() const override;
    void expressionNode() override {}
};

class CallExpression : public Expression {
public:
    CallExpression(const Token& t, std::shared_ptr<Expression> f);
    Token token; // The '(' token
    std::shared_ptr<Expression> Function; // Identifier or FunctionLiteral
    std::vector<std::shared_ptr<Expression>> Arguments;

    std::string TokenLiteral() const override;
    std::string String() const override;
    void expressionNode() override {}
};

class StringLiteral : public Expression {
public: 
    StringLiteral(const Token& t);
    StringLiteral(const Token& t, const std::string& s) : token(t), Value(s) {}
    Token token;
    std::string Value;
    void expressionNode() override {}
    std::string TokenLiteral() const override;
    std::string String() const override;
};

class ArrayLiteral : public Expression {
public:
    Token token;
    ArrayLiteral(const Token& t);
    std::vector<std::shared_ptr<Expression>> Elements;

    void expressionNode() override {}
    std::string TokenLiteral() const override;
    std::string String() const override;
};

class IndexExpression : public Expression {
public:
    IndexExpression(const Token& t, std::shared_ptr<Expression> l);
    Token token; //the [ token
    std::shared_ptr<Expression> Left;
    std::shared_ptr<Expression> Index;

    void expressionNode() override {}
    std::string TokenLiteral() const override;
    std::string String() const override;
};

class HashLiteral : public Expression {
public:
    Token token;
    std::map<std::shared_ptr<Expression>, std::shared_ptr<Expression>> Pairs;
    
    void expressionNode() override {}

    HashLiteral(const Token& t);
    std::string TokenLiteral() const override;
    std::string String() const override;
};

std::string join(const std::vector<std::string>&, const std::string&);

} // namespace YOXS_AST

#endif //AST_H
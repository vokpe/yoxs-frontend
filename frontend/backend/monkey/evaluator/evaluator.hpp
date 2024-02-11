// evaluator.hpp
#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "../ast/ast.hpp"
#include "../object/object.hpp"
#include "../object/environment.hpp"
#include <map>
#include <cstdarg>
#include <cstdio>
#include <memory>

using namespace YOXS_OBJECT;
using namespace YOXS_AST;

class Evaluator {
public:

    static std::shared_ptr<Object> Eval(std::shared_ptr<Node> node, std::shared_ptr<Environment> env);
    static std::shared_ptr<Object> evalProgram(std::shared_ptr<Program> program, std::shared_ptr<Environment> env);
    static std::shared_ptr<Object> evalBlockStatement(std::shared_ptr<BlockStatement> block, std::shared_ptr<Environment> env);
    static std::shared_ptr<BooleanObject> nativeBoolToBooleanObject(bool input);
    static std::shared_ptr<Object> evalPrefixExpression(const std::string& op, std::shared_ptr<Object> right);
    static std::shared_ptr<Object> evalInfixExpression(const std::string& op, std::shared_ptr<Object> left, std::shared_ptr<Object> right);
    static std::shared_ptr<Object> evalBangOperatorExpression(std::shared_ptr<Object> right);
    static std::shared_ptr<Object> evalMinusPrefixOperatorExpression(std::shared_ptr<Object> right);
    static std::shared_ptr<Object> evalIntegerInfixExpression(const std::string& op, std::shared_ptr<Object> left, std::shared_ptr<Object> right);
    static std::shared_ptr<Object> evalStringInfixExpression(const std::string& op, std::shared_ptr<Object> left, std::shared_ptr<Object> right);
    static std::shared_ptr<Object> evalIfExpression(std::shared_ptr<IfExpression> ie, std::shared_ptr<Environment> env);
    static std::shared_ptr<Object> evalIdentifier(std::shared_ptr<Identifier> node, std::shared_ptr<Environment> env);
    
    static bool isTruthy(std::shared_ptr<Object> obj);
    static std::shared_ptr<Error> newError(const std::string format, ...);
    static bool isError(std::shared_ptr<Object> obj);
    static std::vector<std::shared_ptr<Object>> evalExpressions(std::vector<std::shared_ptr<Expression>> exps, std::shared_ptr<Environment> env);
    static std::shared_ptr<Object> applyFunction(std::shared_ptr<Object> fn, std::vector<std::shared_ptr<Object>> args);
    static std::shared_ptr<Environment> extendFunctionEnv(std::shared_ptr<Function> fn, std::vector<std::shared_ptr<Object>> args);
    static std::shared_ptr<Object> unwrapReturnValue(std::shared_ptr<Object> obj);
    static std::shared_ptr<Object> evalIndexExpression(std::shared_ptr<Object> left, std::shared_ptr<Object> index);
    static std::shared_ptr<Object> evalArrayIndexExpression(std::shared_ptr<Object> array, std::shared_ptr<Object> index);
    static std::shared_ptr<Object> evalHashLiteral(std::shared_ptr<HashLiteral> node, std::shared_ptr<Environment> env);
    static std::shared_ptr<Object> evalHashIndexExpression(std::shared_ptr<Object> hash, std::shared_ptr<Object> index);
};

class ObjectConstants {
public:
    static std::shared_ptr<NullObject> NULL_OBJ;
    static std::shared_ptr<BooleanObject> TRUE;
    static std::shared_ptr<BooleanObject> FALSE;
};

#endif // EVALUATOR_H

#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "evaluator/evaluator.hpp"
#include "object/environment.hpp"
#include "object/object.hpp"
#include <iostream>
#include <string>
#include <memory>
#include <cassert>

void TestAddFunction() {
    std::string script = 
        "let add = fn(a, b) { return a + b; };\n"
        "let result = add(5, 5);\n"
        "result;";

    Lexer lexer(script);
    Parser parser(lexer);
    auto program = parser.ParseProgram();
    assert(parser.Errors().empty());

    auto env = std::make_shared<YOXS_OBJECT::Environment>();
    auto evaluated = Evaluator::Eval(program, env);

    auto result = std::dynamic_pointer_cast<YOXS_OBJECT::Integer>(evaluated);
    assert(result != nullptr);
    assert(result->Value == 10);
}

void TestBooleanExpression() {
    std::string script = 
        "let a = true;\n"
        "let b = false;\n"
        "let c = a == b;\n"
        "c;";

    Lexer lexer(script);
    Parser parser(lexer);
    auto program = parser.ParseProgram();
    assert(parser.Errors().empty());

    auto env = std::make_shared<YOXS_OBJECT::Environment>();
    auto evaluated = Evaluator::Eval(program, env);

    auto result = std::dynamic_pointer_cast<YOXS_OBJECT::BooleanObject>(evaluated);
    assert(result != nullptr);
    assert(result->Value == false);
}

void TestStringConcatenation() {
    std::string script = 
        "let hello = 'Hello, ';\n"
        "let world = 'World!';\n"
        "let greeting = hello + world;\n"
        "greeting;";

    Lexer lexer(script);
    Parser parser(lexer);
    auto program = parser.ParseProgram();
    assert(parser.Errors().empty());

    auto env = std::make_shared<YOXS_OBJECT::Environment>();
    auto evaluated = Evaluator::Eval(program, env);

    auto result = std::dynamic_pointer_cast<YOXS_OBJECT::String>(evaluated);
    assert(result != nullptr);
    assert(result->Value == "Hello, World!");
}

void TestIfElseExpression() {
    std::string script = 
        "let x = 10;\n"
        "let y = if (x > 5) { 'greater' } else { 'lesser' };\n"
        "y;";

    Lexer lexer(script);
    Parser parser(lexer);
    auto program = parser.ParseProgram();
    assert(parser.Errors().empty());

    auto env = std::make_shared<YOXS_OBJECT::Environment>();
    auto evaluated = Evaluator::Eval(program, env);

    auto result = std::dynamic_pointer_cast<YOXS_OBJECT::String>(evaluated);
    assert(result != nullptr);
    assert(result->Value == "greater");
}

void TestFunctionApplication() {
    std::string script = 
        "let identity = fn(x) { x; };\n"
        "let result = identity(5);\n"
        "result;";

    Lexer lexer(script);
    Parser parser(lexer);
    auto program = parser.ParseProgram();
    assert(parser.Errors().empty());

    auto env = std::make_shared<YOXS_OBJECT::Environment>();
    auto evaluated = Evaluator::Eval(program, env);

    auto result = std::dynamic_pointer_cast<YOXS_OBJECT::Integer>(evaluated);
    assert(result != nullptr);
    assert(result->Value == 5);
}



int main() {
    TestAddFunction();
    TestBooleanExpression();
    TestStringConcatenation();
    TestIfElseExpression();
    TestFunctionApplication();

    std::cout << "Integration test passed successfully!" << std::endl;
    return 0;
}

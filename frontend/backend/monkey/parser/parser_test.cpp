#include "parser.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <cassert>
#include <variant>

void TestLetStatements();
void TestReturnStatements();
void TestIdentifierExpression();
void TestIntegerLiteralExpression();
void TestParsingPrefixExpressions();
void TestParsingInfixExpressions();
void TestOperatorPrecedenceParsing();
void TestBooleanExpression();
void TestIfExpression();
void TestFunctionLiteralParsing();
void TestFunctionParameterParsing();
void TestCallExpressionParsing();
void TestCallExpressionParameterParsing();
bool testLetStatement(const std::shared_ptr<Statement>& s, const std::string& name);
bool testInfixExpression(const Expression& exp, const std::variant<int, bool, std::string>& left, const std::string& operator_, const std::variant<int, bool, std::string>& right);
bool testLiteralExpression(const Expression& exp, const std::variant<int, bool, std::string>& expected);
bool testIntegerLiteral(const Expression& il, int value);
bool testIdentifier(const Expression& exp, const std::string& value);
bool testBooleanLiteral(const Expression& exp, bool value);
void checkParserErrors(const Parser& p);

void TestLetStatements() {
    struct TestCase {
        std::string input;
        std::string expectedIdentifier;
        std::variant<int, bool, std::string> expectedValue; // Assumes all expected values are int or bool or string.
    };

    std::vector<TestCase> tests = {
        {"let x = 5;", "x", 5},
        {"let y = true;", "y", true},
        {"let foobar = y;", "foobar", "y"}
    };

    for (const auto& tt : tests) {
        Lexer l(tt.input);
        Parser p(l);
        auto program = p.ParseProgram();
        checkParserErrors(p);

        if (program->Statements.size() != 1) {
            std::cerr << "program.Statements does not contain 1 statements. got=" 
                      << program->Statements.size() << std::endl;
            return; // This was a fatal error in Go, so we just return here.
        }

        if (!testLetStatement(program->Statements[0], tt.expectedIdentifier)) {
            return;
        }

        // Safely extract the value from the LetStatement
        auto letStmtPtr = dynamic_cast<LetStatement*>(program->Statements[0].get());
        if (!letStmtPtr) {
            std::cerr << "Statement is not a LetStatement. Got " << typeid(program->Statements[0].get()).name() << std::endl;
            return;
        }

        const Expression& value = *letStmtPtr->Value;
        if (!testLiteralExpression(value, tt.expectedValue)) {
            return;
        }
    }
}

void TestReturnStatements() {
    struct TestCase {
        std::string input;
        std::variant<int, bool, std::string> expectedValue;
    };

    std::vector<TestCase> tests = {
        {"return 5;", 5},
        {"return true;", true},
        {"return foobar;", "foobar"},
    };

    for (const auto& tt : tests) {
        Lexer l(tt.input);
        Parser p(l);
        auto program = p.ParseProgram();
        checkParserErrors(p);

        assert(program->Statements.size() == 1);

        const auto& stmt = program->Statements[0];
        auto returnStmt = dynamic_cast<ReturnStatement*>(stmt.get());
        if (!returnStmt) {
            std::cerr << "stmt not a ReturnStatement. Got " << typeid(stmt).name() << std::endl;
            return;
        }

        assert(returnStmt->TokenLiteral() == "return");

        const Expression& value = *returnStmt->ReturnValue;
        if (!testLiteralExpression(value, tt.expectedValue)) {
            return;
        }
    }
}

void TestIdentifierExpression() {
    const std::string input = "foobar;";

    Lexer l(input);
    Parser p(l);
    auto program = p.ParseProgram();
    checkParserErrors(p);

    assert(program->Statements.size() == 1);

    const auto* exprStmt = dynamic_cast<ExpressionStatement*>(program->Statements[0].get());
    if (!exprStmt) {
        std::cerr << "program.Statements[0] is not ExpressionStatement. Got "
                  << typeid(program->Statements[0].get()).name() << std::endl;

        return;
    }

    const auto* ident = dynamic_cast<Identifier*>(exprStmt->expr.get());
    if (!ident) {
        std::cerr << "exp not Identifier. Got "
                  << typeid(exprStmt->expr.get()).name() << std::endl;
        return;
    }

    assert(ident->Value() == "foobar"); // Ensure Identifier has a Value method or appropriate member variable
    assert(ident->TokenLiteral() == "foobar");
}

void TestIntegerLiteralExpression() {
    const std::string input = "5;";

    Lexer l(input);
    Parser p(l);
    auto program = p.ParseProgram();
    checkParserErrors(p);

    assert(program->Statements.size() == 1);

    const auto* exprStmt = dynamic_cast<ExpressionStatement*>(program->Statements[0].get());
    if (!exprStmt) {

        std::cerr << "program.Statements[0] is not ExpressionStatement. Got "
                  << typeid(program->Statements[0].get()).name() << std::endl;

        return;
    }

    const auto* literal = dynamic_cast<IntegerLiteral*>(exprStmt->expr.get());
    if (!literal) {
        std::cerr << "exp not IntegerLiteral. Got "
                  << typeid(exprStmt->expr.get()).name() << std::endl;
        return;
    }

    assert(literal->Value == 5); // Ensure IntegerLiteral has a Value member
    assert(literal->TokenLiteral() == "5");
}



void TestParsingPrefixExpressions() {

    struct PrefixTest {
        std::string input;
        std::string oper;
        std::variant<int, bool, std::string> value;
    };

    std::vector<PrefixTest> prefixTests = {
        {"!5;", "!", 5},
        {"-15;", "-", 15},
        {"!foobar;", "!", "foobar"},
        {"-foobar;", "-", "foobar"},
        {"!true;", "!", true},
        {"!false;", "!", false},
    };

    for (const auto& tt : prefixTests) {
        Lexer l(tt.input);
        Parser p(l);
        auto program = p.ParseProgram();
        checkParserErrors(p);

        assert(program->Statements.size() == 1);

        const auto* exprStmt = dynamic_cast<ExpressionStatement*>(program->Statements[0].get());
        if (!exprStmt) {
            std::cerr << "program.Statements[0] is not ExpressionStatement. Got "
                      << typeid(program->Statements[0].get()).name() << std::endl;
            return;
        }

        const auto* exp = dynamic_cast<PrefixExpression*>(exprStmt->expr.get());
        if (!exp) {
            std::cerr << "stmt is not PrefixExpression. Got "
                      << typeid(exprStmt->expr.get()).name() << std::endl;
            return;
        }

        assert(exp->Operator == tt.oper);

        if (!testLiteralExpression(*(exp->Right), tt.value)) {
            return;
        }

    }
}

void TestParsingInfixExpressions() {

    struct InfixTest {
        std::string input;
        std::variant<int, bool, std::string> leftValue;
        std::string oper;
        std::variant<int, bool, std::string> rightValue;
    };

    std::vector<InfixTest> infixTests = {
        {"5 + 5;", 5, "+", 5},
		{"5 - 5;", 5, "-", 5},
		{"5 * 5;", 5, "*", 5},
		{"5 / 5;", 5, "/", 5},
		{"5 > 5;", 5, ">", 5},
		{"5 < 5;", 5, "<", 5},
		{"5 == 5;", 5, "==", 5},
		{"5 != 5;", 5, "!=", 5},
		{"foobar + barfoo;", "foobar", "+", "barfoo"},
		{"foobar - barfoo;", "foobar", "-", "barfoo"},
		{"foobar * barfoo;", "foobar", "*", "barfoo"},
		{"foobar / barfoo;", "foobar", "/", "barfoo"},
		{"foobar > barfoo;", "foobar", ">", "barfoo"},
		{"foobar < barfoo;", "foobar", "<", "barfoo"},
		{"foobar == barfoo;", "foobar", "==", "barfoo"},
		{"foobar != barfoo;", "foobar", "!=", "barfoo"},
		{"true == true", true, "==", true},
		{"true != false", true, "!=", false},
		{"false == false", false, "==", false}
    };

    for (const auto& tt : infixTests) {
        Lexer l(tt.input);
        Parser p(l);
        auto program = p.ParseProgram();
        checkParserErrors(p);

        assert(program->Statements.size() == 1);

        const auto* exprStmt = dynamic_cast<ExpressionStatement*>(program->Statements[0].get());
        if (!exprStmt) {
            std::cerr << "program.Statements[0] is not ExpressionStatement. Got "
                      << typeid(program->Statements[0].get()).name() << std::endl;
            return;
        }

        if (!testInfixExpression(*exprStmt->expr, tt.leftValue, tt.oper, tt.rightValue)) {
            return;
        }
    }
}

void TestOperatorPrecedenceParsing() {
    struct TestCase {
        std::string input;
        std::string expected;
    };

    std::vector<TestCase> tests = {
        {
			"-a * b",
			"((-a) * b)",
		},
		{
			"!-a",
			"(!(-a))",
		},
		{
			"a + b + c",
			"((a + b) + c)",
		},
		{
			"a + b - c",
			"((a + b) - c)",
		},
		{
			"a * b * c",
			"((a * b) * c)",
		},
		{
			"a * b / c",
			"((a * b) / c)",
		},
		{
			"a + b / c",
			"(a + (b / c))",
		},
		{
			"a + b * c + d / e - f",
			"(((a + (b * c)) + (d / e)) - f)",
		},
		{
			"3 + 4; -5 * 5",
			"(3 + 4)((-5) * 5)",
		},
		{
			"5 > 4 == 3 < 4",
			"((5 > 4) == (3 < 4))",
		},
		{
			"5 < 4 != 3 > 4",
			"((5 < 4) != (3 > 4))",
		},
		{
			"3 + 4 * 5 == 3 * 1 + 4 * 5",
			"((3 + (4 * 5)) == ((3 * 1) + (4 * 5)))",
		},
		{
			"true",
			"true",
		},
		{
			"false",
			"false",
		},
		{
			"3 > 5 == false",
			"((3 > 5) == false)",
		},
		{
			"3 < 5 == true",
			"((3 < 5) == true)",
		},
		{
			"1 + (2 + 3) + 4",
			"((1 + (2 + 3)) + 4)",
		},
		{
			"(5 + 5) * 2",
			"((5 + 5) * 2)",
		},
		{
			"2 / (5 + 5)",
			"(2 / (5 + 5))",
		},
		{
			"(5 + 5) * 2 * (5 + 5)",
			"(((5 + 5) * 2) * (5 + 5))",
		},
		{
			"-(5 + 5)",
			"(-(5 + 5))",
		},
		{
			"!(true == true)",
			"(!(true == true))",
		},
		{
			"a + add(b * c) + d",
			"((a + add((b * c))) + d)",
		},
		{
			"add(a, b, 1, 2 * 3, 4 + 5, add(6, 7 * 8))",
			"add(a, b, 1, (2 * 3), (4 + 5), add(6, (7 * 8)))",
		},
		{
			"add(a + b + c * d / f + g)",
			"add((((a + b) + ((c * d) / f)) + g))",
		},
        {
			"a * [1, 2, 3, 4][b * c] * d",
			"((a * ([1, 2, 3, 4][(b * c)])) * d)",
		},
		{
			"add(a * b[2], b[1], 2 * [1, 2][1])",
			"add((a * (b[2])), (b[1]), (2 * ([1, 2][1])))",
		}
    };

    for (const auto& tt : tests) {
        Lexer l(tt.input);
        Parser p(l);
        auto program = p.ParseProgram();
        checkParserErrors(p);

        std::string actual = program->String();
        if (actual != tt.expected) {
            std::cerr << "expected=" << tt.expected << ", got=" << actual << std::endl;
        }
    }
}

void TestBooleanExpression() {
    struct TestCase {
        std::string input;
        bool expectedBoolean;
    };

    std::vector<TestCase> tests = {
        {"true;", true},
        {"false;", false},
    };

    for (const auto& tt : tests) {
        Lexer l(tt.input);
        Parser p(l);
        auto program = p.ParseProgram();
        checkParserErrors(p);

        if (program->Statements.size() != 1) {
            std::cerr << "program.Statements does not contain 1 statements. got=" 
                      << program->Statements.size() << std::endl;
            return; // This was a fatal error in Go, so we just return here.
        }

        auto* exprStmt = dynamic_cast<ExpressionStatement*>(program->Statements[0].get());
        if (!exprStmt) {
            std::cerr << "program.Statements[0] is not ExpressionStatement. Got "
                      << typeid(program->Statements[0].get()).name() << std::endl;

            return;
        }

        auto* boolean = dynamic_cast<Boolean*>(exprStmt->expr.get());
        if (!boolean) {
            std::cerr << "exp not Boolean. Got "
                      << typeid(exprStmt->expr.get()).name() << std::endl;
            return;
        }

        if (boolean->Value != tt.expectedBoolean) {
            std::cerr << "boolean.Value not " << tt.expectedBoolean
                      << ". got=" << boolean->Value << std::endl;
        }
    }
}

void TestIfExpression() {
    std::string input = "if (x < y) { x }";

    Lexer l(input);
    Parser p(l);
    auto program = p.ParseProgram();
    checkParserErrors(p);

    if (program->Statements.size() != 1) {
        std::cerr << "program.Statements does not contain 1 statements. got=" 
                  << program->Statements.size() << std::endl;
        return;
    }

    auto* stmt = dynamic_cast<ExpressionStatement*>(program->Statements[0].get());
    if (!stmt) {
        std::cerr << "program.Statements[0] is not ExpressionStatement. got=" 
                  << typeid(program->Statements[0].get()).name() << std::endl;
        return;
    }

    auto* exp = dynamic_cast<IfExpression*>(stmt->expr.get());
    if (!exp) {
        std::cerr << "stmt.Expression is not IfExpression. got=" 
                  << typeid(stmt->expr.get()).name() << std::endl;
        return;
    }

    if (!testInfixExpression(*exp->Condition, "x", "<", "y")) {
        return;
    }

    if (exp->Consequence->Statements.size() != 1) {
        std::cerr << "consequence is not 1 statements. got=" 
                  << exp->Consequence->Statements.size() << std::endl;
        return;
    }

    auto* consequence = dynamic_cast<ExpressionStatement*>(exp->Consequence->Statements[0].get());
    if (!consequence) {
        std::cerr << "Statements[0] is not ExpressionStatement. got=" 
                  << typeid(exp->Consequence->Statements[0].get()).name() << std::endl;
        return;
    }

    if (!testIdentifier(*consequence->expr, "x")) {
        return;
    }

    if (exp->Alternative) {
        std::cerr << "exp.Alternative.Statements was not nullptr. got some content." << std::endl;
        return;
    }
}

void TestIfElseExpression() {
    std::string input = "if (x < y) { x } else { y }";

    Lexer l(input);
    Parser p(l);
    std::shared_ptr<Program> program = p.ParseProgram();
    checkParserErrors(p);

    if (program->Statements.size() != 1) {
        std::cerr << "program.Statements does not contain 1 statements. got=" 
                  << program->Statements.size() << std::endl;
        return;
    }

    auto* stmt = dynamic_cast<ExpressionStatement*>(program->Statements[0].get());
    if (!stmt) {
        std::cerr << "program.Statements[0] is not ExpressionStatement. got=" 
                  << typeid(program->Statements[0].get()).name() << std::endl;
        return;
    }

    auto* exp = dynamic_cast<IfExpression*>(stmt->expr.get());
    if (!exp) {
        std::cerr << "stmt.Expression is not IfExpression. got=" 
                  << typeid(stmt->expr.get()).name() << std::endl;
        return;
    }

    if (!testInfixExpression(*exp->Condition, "x", "<", "y")) {
        return;
    }

    if (exp->Consequence->Statements.size() != 1) {
        std::cerr << "consequence is not 1 statements. got=" 
                  << exp->Consequence->Statements.size() << std::endl;
        return;
    }

    auto* consequence = dynamic_cast<ExpressionStatement*>(exp->Consequence->Statements[0].get());
    if (!consequence) {
        std::cerr << "Statements[0] is not ExpressionStatement. got=" 
                  << typeid(exp->Consequence->Statements[0].get()).name() << std::endl;
        return;
    }

    if (!testIdentifier(*consequence->expr, "x")) {
        return;
    }

    if (exp->Alternative && exp->Alternative->Statements.size() != 1) {
        std::cerr << "exp.Alternative.Statements does not contain 1 statements. got=" 
                  + std::to_string(exp->Alternative->Statements.size()) << std::endl;
        return;
    }

    auto* alternative = dynamic_cast<ExpressionStatement*>(exp->Alternative->Statements[0].get());
    if(!alternative) {
        std::cerr << "Statements[0] is not ast.ExpressionStatement. got="
                  << typeid(exp->Alternative->Statements[0].get()).name() << std::endl;
    }

    if (!testIdentifier(*alternative->expr, "y")) {
		return;
	}
}

void TestFunctionLiteralParsing() {
    std::string input = "fn(x, y) { x + y; }";

    Lexer l(input);
    Parser p(l);
    std::shared_ptr<Program> program = p.ParseProgram();
    checkParserErrors(p);

    if (program->Statements.size() != 1) {
        std::cerr << "program.Statements does not contain 1 statements. got=" 
                  << program->Statements.size() << std::endl;
        return;
    }

    auto* exprStmt = dynamic_cast<ExpressionStatement*>(program->Statements[0].get());
    if (!exprStmt) {
        std::cerr << "program.Statements[0] is not ExpressionStatement. Got "
                  << typeid(program->Statements[0].get()).name() << std::endl;
        return;
    }

    auto* function = dynamic_cast<FunctionLiteral*>(exprStmt->expr.get());
    if (!function) {
        std::cerr << "stmt.Expression is not FunctionLiteral. Got "
                  << typeid(exprStmt->expr.get()).name() << std::endl;
        return;
    }

    if (function->Parameters.size() != 2) {
        std::cerr << "function literal parameters wrong. want 2, got="
                  << function->Parameters.size() << std::endl;
        return;
    }

    testLiteralExpression(*function->Parameters[0].get(), "x");
    testLiteralExpression(*function->Parameters[1].get(), "y");

    if (function->Body->Statements.size() != 1) {
        std::cerr << "function.Body.Statements has not 1 statements. got=" 
                  << function->Body->Statements.size() << std::endl;
        return;
    }

    auto* bodyStmt = dynamic_cast<ExpressionStatement*>(function->Body->Statements[0].get());
    if (!bodyStmt) {
        std::cerr << "function body stmt is not ExpressionStatement. Got "
                  << typeid(function->Body->Statements[0].get()).name() << std::endl;
        return;
    }

    testInfixExpression(*bodyStmt->expr.get(), "x", "+", "y");
}

void TestFunctionParameterParsing() {
    struct Test {
        std::string input;
        std::vector<std::string> expectedParams;
    };

    std::vector<Test> tests = {
        {"fn() {};", {}},
        {"fn(x) {};", {"x"}},
        {"fn(x, y, z) {};", {"x", "y", "z"}}
    };

    for (const auto& tt : tests) {
        Lexer l(tt.input);
        Parser p(l);
        std::shared_ptr<Program> program = p.ParseProgram();
        checkParserErrors(p); // Assuming this function is modified for C++

        auto* stmt = dynamic_cast<ExpressionStatement*>(program->Statements[0].get());
        if (!stmt) {
            std::cerr << "Statement is not ExpressionStatement." << std::endl;
            return;
        }

        auto* function = dynamic_cast<FunctionLiteral*>(stmt->expr.get());
        if (!function) {
            std::cerr << "Expression is not FunctionLiteral." << std::endl;
            return;
        }

        if (function->Parameters.size() != tt.expectedParams.size()) {
            std::cerr << "length parameters wrong. want " 
                      << tt.expectedParams.size() 
                      << ", got=" << function->Parameters.size() << std::endl;
        }

        for (size_t i = 0; i < tt.expectedParams.size(); ++i) {
            testLiteralExpression(*function->Parameters[i].get(), tt.expectedParams[i]);
        }
    }
}

void TestCallExpressionParsing() {
    std::string input = "add(1, 2 * 3, 4 + 5);";

    Lexer l(input);
    Parser p(l);
    std::shared_ptr<Program> program = p.ParseProgram();
    checkParserErrors(p); // Assuming this function is modified for C++

    if (program->Statements.size() != 1) {
        std::cerr << "program.Statements does not contain " << 1 
                  << " statements. got=" << program->Statements.size() << std::endl;
        return;
    }

    auto* stmt = dynamic_cast<ExpressionStatement*>(program->Statements[0].get());
    if (!stmt) {
        std::cerr << "stmt is not ExpressionStatement. got=" 
                  << typeid(program->Statements[0].get()).name() << std::endl;
        return;
    }

    auto* exp = dynamic_cast<CallExpression*>(stmt->expr.get());
    if (!exp) {
        std::cerr << "stmt.Expression is not CallExpression. got=" 
                  << typeid(stmt->expr.get()).name() << std::endl;
        return;
    }

    if (!testIdentifier(*exp->Function.get(), "add")) {
        return;
    }

    if (exp->Arguments.size() != 3) {
        std::cerr << "wrong length of arguments. got=" << exp->Arguments.size() << std::endl;
        return;
    }

    testLiteralExpression(*exp->Arguments[0].get(), 1);
    testInfixExpression(*exp->Arguments[1].get(), 2, "*", 3);
    testInfixExpression(*exp->Arguments[2].get(), 4, "+", 5);
}

void TestCallExpressionParameterParsing() {
    struct TestCase {
        std::string input;
        std::string expectedIdent;
        std::vector<std::string> expectedArgs;
    };

    std::vector<TestCase> tests = {
        {"add();", "add", {}},
        {"add(1);", "add", {"1"}},
        {"add(1, 2 * 3, 4 + 5);", "add", {"1", "(2 * 3)", "(4 + 5)"}}
    };

    for (const auto& tt : tests) {
        Lexer l(tt.input);
        Parser p(l);
        std::shared_ptr<Program> program = p.ParseProgram();
        checkParserErrors(p); // Assuming this function is adapted for C++

        auto* stmt = dynamic_cast<ExpressionStatement*>(program->Statements[0].get());
        if (!stmt) {
            std::cerr << "First statement is not an ExpressionStatement. got=" 
                      << typeid(program->Statements[0].get()).name() << std::endl;
            return;
        }

        auto* exp = dynamic_cast<CallExpression*>(stmt->expr.get());
        if (!exp) {
            std::cerr << "stmt.Expression is not CallExpression. got=" 
                      << typeid(stmt->expr.get()).name() << std::endl;
            return;
        }

        if (!testIdentifier(*exp->Function.get(), tt.expectedIdent)) {
            return;
        }

        if (exp->Arguments.size() != tt.expectedArgs.size()) {
            std::cerr << "wrong number of arguments. want=" << tt.expectedArgs.size() 
                      << ", got=" << exp->Arguments.size() << std::endl;
            return;
        }

        for (size_t i = 0; i < tt.expectedArgs.size(); ++i) {
            if (exp->Arguments[i]->String() != tt.expectedArgs[i]) {
                std::cerr << "argument " << i << " wrong. want=" << tt.expectedArgs[i] 
                          << ", got=" << exp->Arguments[i]->String() << std::endl;
            }
        }
    }
}

void TestStringLiteralExpression() {
    std::string input = "\"Hello World\";";

    Lexer l(input);
    Parser p(l);
    auto program = p.ParseProgram();
    checkParserErrors(p);

    assert(program->Statements.size() == 1);

    const auto* exprStmt = dynamic_cast<ExpressionStatement*>(program->Statements[0].get());
    assert(exprStmt != nullptr);

    const auto* literal = dynamic_cast<StringLiteral*>(exprStmt->expr.get());
    assert(literal != nullptr);
    std::cout << "got literal: " + literal->String() << std::endl;
    assert(literal->String() == "Hello World");
}

void TestArrayLiteralExpression() {
    std::string input = "[1, 2];";

    Lexer l(input);
    Parser p(l);
    auto program = p.ParseProgram();
    checkParserErrors(p);

    assert(program->Statements.size() == 1);

    const auto* exprStmt = dynamic_cast<ExpressionStatement*>(program->Statements[0].get());
    assert(exprStmt != nullptr);

    const auto* array = dynamic_cast<ArrayLiteral*>(exprStmt->expr.get());
    assert(array != nullptr);
    assert(array->Elements.size() == 2);
    testIntegerLiteral(*array->Elements[0], 1);
    testIntegerLiteral(*array->Elements[1], 2);
}

void TestIndexExpressions() {
    std::string input = "myArray[0];";

    Lexer l(input);
    Parser p(l);
    auto program = p.ParseProgram();
    checkParserErrors(p);

    assert(program->Statements.size() == 1);

    const auto* exprStmt = dynamic_cast<ExpressionStatement*>(program->Statements[0].get());
    assert(exprStmt != nullptr);

    const auto* indexExp = dynamic_cast<IndexExpression*>(exprStmt->expr.get());
    assert(indexExp != nullptr);

    testIdentifier(*indexExp->Left, "myArray");
    testIntegerLiteral(*indexExp->Index, 0);
}

void TestHashLiteralExpression() {
    std::string input = "{\"key\": \"value\"};";

    Lexer l(input);
    Parser p(l);
    auto program = p.ParseProgram();
    checkParserErrors(p);

    assert(program->Statements.size() == 1);

    const auto* exprStmt = dynamic_cast<ExpressionStatement*>(program->Statements[0].get());
    assert(exprStmt != nullptr);

    const auto* hash = dynamic_cast<HashLiteral*>(exprStmt->expr.get());
    assert(hash != nullptr);
    assert(hash->Pairs.size() == 1);

    const auto* key = dynamic_cast<StringLiteral*>(hash->Pairs.begin()->first.get());
    const auto* value = dynamic_cast<StringLiteral*>(hash->Pairs.begin()->second.get());
    assert(key != nullptr && key->String() == "key");
    assert(value != nullptr && value->String() == "value");
}

bool testLetStatement(const std::shared_ptr<Statement>& s, const std::string& name) {
    if (s->TokenLiteral() != "let") {
        std::cerr << "s.TokenLiteral not 'let'. got=" << s->TokenLiteral() << std::endl;
        return false;
    }

    const LetStatement* letStmt = dynamic_cast<LetStatement*>(s.get());
    if (!letStmt) {
        std::cerr << "s not LetStatement. got=" << typeid(s.get()).name() << std::endl;
        return false;
    }

    if (letStmt->Name->Value() != name) {
        std::cerr << "letStmt.Name.Value not '" << name << "'. got=" << letStmt->Name->Value() << std::endl;
        return false;
    }

    if (letStmt->Name->TokenLiteral() != name) {
        std::cerr << "letStmt.Name.TokenLiteral() not '" << name << "'. got=" << letStmt->Name->TokenLiteral() << std::endl;
        return false;
    }

    return true;
}

bool testInfixExpression(const Expression& exp, const std::variant<int, bool, std::string>& left, const std::string& operator_, const std::variant<int, bool, std::string>& right) {
    const InfixExpression* opExp = dynamic_cast<const InfixExpression*>(&exp);
    if (!opExp) {
        std::cerr << "exp is not InfixExpression. got=" << typeid(exp).name() << "(" << exp.String() << ")" << std::endl;
        return false;
    }

    if (!testLiteralExpression(*opExp->Left, left)) {
        return false;
    }

    if (opExp->Operator != operator_) {
        std::cerr << "opExp.Operator is not '" << operator_ << "'. got=" << opExp->Operator << std::endl;
        return false;
    }

    if (!testLiteralExpression(*opExp->Right, right)) {
        return false;
    }

    return true;
}

bool testLiteralExpression(const Expression& exp, const std::variant<int, bool, std::string>& expected) {
    switch (expected.index()) {
        case 0: return testIntegerLiteral(exp, std::get<int>(expected));
        case 1: return testBooleanLiteral(exp, std::get<bool>(expected));
        case 2: return testIdentifier(exp, std::get<std::string>(expected));
        default:
            std::cerr << "type of exp not handled. got=" << typeid(exp).name() << std::endl;
            return false;
    }
}

bool testIntegerLiteral(const Expression& il, int value) {
    const IntegerLiteral* integ = dynamic_cast<const IntegerLiteral*>(&il);
    if (!integ) {
        std::cerr << "il not IntegerLiteral. got=" << typeid(il).name() << std::endl;
        return false;
    }

    if (integ->Value != value) {
        std::cerr << "integ.Value not " << value << ". got=" << integ->Value << std::endl;
        return false;
    }

    if (integ->TokenLiteral() != std::to_string(value)) {
        std::cerr << "integ.TokenLiteral not " << value << ". got=" << integ->TokenLiteral() << std::endl;
        return false;
    }

    return true;
}

bool testIdentifier(const Expression& exp, const std::string& value) {
    const Identifier* ident = dynamic_cast<const Identifier*>(&exp);
    if (!ident) {
        std::cerr << "exp not Identifier. got=" << typeid(exp).name() << std::endl;
        return false;
    }

    if (ident->Value() != value) {
        std::cerr << "ident.Value not " << value << ". got=" << ident->Value() << std::endl;
        return false;
    }

    if (ident->TokenLiteral() != value) {
        std::cerr << "ident.TokenLiteral not " << value << ". got=" << ident->TokenLiteral() << std::endl;
        return false;
    }

    return true;
}


bool testBooleanLiteral(const Expression& exp, bool value) {
    const Boolean* bo = dynamic_cast<const Boolean*>(&exp);
    if (!bo) {
        std::cerr << "exp not Boolean. got=" << typeid(exp).name() << std::endl;
        return false;
    }

    if (bo->Value != value) {
        std::cerr << "bo.Value not " << value << ". got=" << bo->Value << std::endl;
        return false;
    }

    if (bo->TokenLiteral() != (value ? "true" : "false")) {
        std::cerr << "bo.TokenLiteral not " << (value ? "true" : "false") << ". got=" << bo->TokenLiteral() << std::endl;
        return false;
    }

    return true;
}

void checkParserErrors(const Parser& p) {
    const auto& errors = p.Errors();
    if (errors.empty()) {
        return;
    }

    std::cerr << "parser has " << errors.size() << " errors" << std::endl;
    for (const auto& error : errors) {
        std::cerr << "parser error: " << error << std::endl;
    }

    assert(false); // Halt execution if there are parser errors.
}

int main() {
    TestLetStatements();
    TestReturnStatements();
    TestIdentifierExpression();
    TestParsingPrefixExpressions();
    TestParsingInfixExpressions();
    TestOperatorPrecedenceParsing();
    TestBooleanExpression();
    TestIfExpression();
    TestFunctionLiteralParsing();
    TestFunctionParameterParsing();
    TestCallExpressionParsing();
    TestCallExpressionParameterParsing();
    TestIntegerLiteralExpression();
    TestParsingInfixExpressions();
    TestStringLiteralExpression();
    TestArrayLiteralExpression();
    TestIndexExpressions();
    TestHashLiteralExpression();
    
    std::cout << "All parser_test.cpp tests passed!" << std::endl;
    return 0;
}

//g++ -std=c++17 -Isrc -o parser_test src/monkey/parser/parser.cpp src/monkey/lexer/lexer.cpp src/monkey/token/token.cpp src/monkey/parser/parser_test.cpp && ./parser_test
//in cpp code file

// g++ -std=c++17 -Isrc -c src/monkey/parser/parser.cpp -o parser.o
// g++ -std=c++17 -Isrc -c src/monkey/lexer/lexer.cpp -o lexer.o
// g++ -std=c++17 -Isrc -c src/monkey/token/token.cpp -o token.o
// g++ -std=c++17 -Isrc -c src/monkey/parser/parser_test.cpp -o parser_test.o
// g++ -std=c++17 -Isrc -c src/monkey/ast/ast.cpp -o ast.o
// g++ -o parser_test parser.o lexer.o token.o ast.o parser_test.o
// ./parser_test
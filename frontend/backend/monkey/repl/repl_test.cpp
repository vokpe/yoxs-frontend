#include "repl.hpp"
#include <sstream>
#include <cassert>

//REPL Test: This tests the REPL (Read-Eval-Print Loop) functionality, ensuring it can read inputs, evaluate them, and print results as expected.

// Function declarations
void testTokenREPL();
void testParserREPL();
void testSimpleExpression();
void testFunctionDefinition();
void testLetStatements();
void testParsingErrors();

int main() {
    // This stringstream will simulate the in put for the REPL.
    testTokenREPL();
    testParserREPL();

    std::cout << "All repl_test.cpp tests passed!" << std::endl;
    return 0;
}

void testTokenREPL() {
    // This stringstream will simulate the input for the REPL.
    std::stringstream input;
    input << "let x = 5;\n";
    input << "let y = 10;\n";
    input << "x + y;\n";

    // This stringstream will capture the REPL's output.
    std::stringstream output;

    // Start the REPL with the mock input and output.
    REPL::tokenStart(input, output);

    // Here, you can check the output to ensure it's what you expect.
    // For now, I'll just check if it contains certain token outputs.
    // Depending on your REPL's verbosity, you might want to adjust these checks.
    std::string replOutput = output.str();
    std::cout << "REPL Output: " << replOutput << std::endl;

    
    assert(replOutput.find("LET") != std::string::npos);
    assert(replOutput.find("x") != std::string::npos);
    assert(replOutput.find("5") != std::string::npos);
    assert(replOutput.find("+") != std::string::npos);
    assert(replOutput.find("y") != std::string::npos);
    assert(replOutput.find("10") != std::string::npos);

    std::cout << "Token REPL tests passed!" << std::endl;
}

void testParserREPL() {
    testLetStatements(); 
    testSimpleExpression();
    testFunctionDefinition();
    testParsingErrors();

    std::cout << "All Parser tests passed!" << std::endl;
}

// This test checks basic arithmetic and let statements
void testLetStatements() {
    std::stringstream input;
    input << "let x = 5;\n";
    input << "let y = 10;\n";
    input << "x + y;\n";

    std::stringstream output;
    REPL::parserStart(input, output);

    std::string replOutput = output.str();
    //std::cout << "REPL Output for Let Statements: " << replOutput << std::endl;

    assert(replOutput.find("let x = 5;") != std::string::npos);
    assert(replOutput.find("let y = 10;") != std::string::npos);
    assert(replOutput.find("(x + y)") != std::string::npos); // Adjusted for AST representation

    std::cout << "let statement tests passed!" << std::endl;
}

void testSimpleExpression() {
    std::istringstream input("5 + 5 * 2;");
    std::ostringstream output;
    REPL::parserStart(input, output);
    assert(output.str().find("(5 + (5 * 2))") != std::string::npos);

    std::cout << "Simple expression tests passed!" << std::endl;
}

void testFunctionDefinition() {
    std::istringstream input("fn(x, y) { x + y; }");
    std::ostringstream output;
    REPL::parserStart(input, output);
    //std::cout << "Function Definition REPL Output: " << output.str() << std::endl;
    //std::string replOutput = output.str();
    assert(output.str().find("fn(x, y) (x + y)") != std::string::npos);
    std::cout << "Function definition tests passed!" << std::endl;
}

void testParsingErrors() {
    std::istringstream input("let x 5;");
    std::ostringstream output;
    REPL::parserStart(input, output);
    std::string expectedErrorOutput = 
        "Woops! We ran into an error:\n"
        "\texpected next token to be =, got INT instead\n"; 
    //std::cout << "Parsing Errors Output: " << output.str() << std::endl;
    assert(output.str().find(expectedErrorOutput) != std::string::npos);
    std::cout << "Parsing error tests passed!" << std::endl;
}

//g++ -std=c++17 -Isrc -o repl_test src/monkey/repl/repl.cpp src/monkey/lexer/lexer.cpp src/monkey/token/token.cpp src/monkey/parser/parser.cpp src/monkey/ast/ast.cpp src/monkey/object/object.cpp src/monkey/evaluator/evaluator.cpp src/monkey/object/environment.cpp src/monkey/repl/repl_test.cpp && ./repl_test
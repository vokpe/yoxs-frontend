# src/monkey/Makefile

# include ../../common.mk
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

AST_DIR := ast
EVALUATOR_DIR := evaluator
LEXER_DIR := lexer
PARSER_DIR := parser
TOKEN_DIR := token
REPL_DIR := repl
OBJECT_DIR := object

.PHONY: all build clean tests token_test lexer_test ast_test parser_test object_test evaluator_test repl_test

all: build tests

build:
	@echo "Build commands for monkey components"

tests: token_test lexer_test ast_test parser_test object_test evaluator_test repl_test #integration_test_p

token_test:
	$(CXX) $(CXXFLAGS) -I. $(TOKEN_DIR)/token_test.cpp $(TOKEN_DIR)/token.cpp -o token_test.out
	./token_test.out

lexer_test:
	$(CXX) $(CXXFLAGS) -I. $(LEXER_DIR)/lexer_test.cpp $(LEXER_DIR)/lexer.cpp $(TOKEN_DIR)/token.cpp -o lexer_test.out
	./lexer_test.out

ast_test:
	$(CXX) $(CXXFLAGS) -I. $(AST_DIR)/ast_test.cpp $(AST_DIR)/ast.cpp $(TOKEN_DIR)/token.cpp -o ast_test.out
	./ast_test.out

parser_test:
	$(CXX) $(CXXFLAGS) -I. $(PARSER_DIR)/parser_test.cpp $(PARSER_DIR)/parser.cpp $(LEXER_DIR)/lexer.cpp $(TOKEN_DIR)/token.cpp $(AST_DIR)/ast.cpp -o parser_test.out
	./parser_test.out

object_test:
	$(CXX) $(CXXFLAGS) -I. $(OBJECT_DIR)/object_test.cpp $(AST_DIR)/ast.cpp $(OBJECT_DIR)/object.cpp -o object_test.out
	./object_test.out

evaluator_test:
	$(CXX) $(CXXFLAGS) -I. $(EVALUATOR_DIR)/evaluator_test.cpp $(PARSER_DIR)/parser.cpp $(LEXER_DIR)/lexer.cpp $(TOKEN_DIR)/token.cpp $(AST_DIR)/ast.cpp $(OBJECT_DIR)/object.cpp $(OBJECT_DIR)/environment.cpp $(EVALUATOR_DIR)/evaluator.cpp -o evaluator_test.out
	./evaluator_test.out

repl_test:
	$(CXX) $(CXXFLAGS) -I. $(REPL_DIR)/repl_test.cpp $(REPL_DIR)/repl.cpp $(LEXER_DIR)/lexer.cpp $(TOKEN_DIR)/token.cpp $(PARSER_DIR)/parser.cpp $(AST_DIR)/ast.cpp $(EVALUATOR_DIR)/evaluator.cpp $(OBJECT_DIR)/environment.cpp $(OBJECT_DIR)/object.cpp -o repl_test.out
	./repl_test.out

clean:
	rm -f *.out *.o

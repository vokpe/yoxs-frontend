#ifndef COMPILER_H
#define COMPILER_H

#include "../ast/ast.hpp"
#include "../code/code.hpp"
#include "../object/object.hpp"
#include "SymbolTable.hpp" 

class Compiler {
public:
    std::vector<std::shared_ptr<Object>> constants;
    std::unique_ptr<SymbolTable> symbolTable;

    struct EmittedInstruction {
        Opcode opcode;
        int position;
    };

    struct CompilationScope {
        Instructions instructions;
        EmittedInstruction lastInstruction;
        EmittedInstruction previousInstruction;
    };

    std::vector<CompilationScope> scopes;
    int scopeIndex;

    Compiler() : scopeIndex(0) {
        CompilationScope mainScope;
        symbolTable = std::make_unique<SymbolTable>();

        // Populate built-ins in the symbol table
        int index = 0;
        for (auto& builtin : Object::Builtins) {
            symbolTable->DefineBuiltin(index++, builtin.name);
        }

        scopes.push_back(mainScope);
    }

    Compiler(std::unique_ptr<SymbolTable> s, std::vector<std::shared_ptr<Object>> consts) : Compiler() {
        symbolTable = std::move(s);
        constants = consts;
    }

    Error Compile(const std::shared_ptr<ast::Node>& node) {
        // Implementation of Compile based on the specific AST node type
    }

    std::shared_ptr<Bytecode> Bytecode() {
        return std::make_shared<Bytecode>(scopes[scopeIndex].instructions, constants);
    }


#endif // COMPILER_H
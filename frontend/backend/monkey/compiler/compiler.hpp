#ifndef COMPILER_H
#define COMPILER_H

#include "../ast/ast.hpp"
#include "../code/code.hpp"
#include "../object/object.hpp"
#include "symbol_table.hpp" 

class Compiler {
public:
    std::vector<std::shared_ptr<YOXS_OBJECT::Object>> constants;
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

    void emit(Opcode op, std::vector<int> operands = {}) {
        Instructions ins = Make(op, operands);
        int pos = addInstruction(ins);
        setLastInstruction(op, pos);
    }

    int addInstruction(const Instructions& ins) {
        auto& currentInstructions = scopes[scopeIndex].instructions;
        int posNewInstruction = currentInstructions.size();
        currentInstructions.insert(currentInstructions.end(), ins.begin(), ins.end());
        return posNewInstruction;
    }

    void setLastInstruction(Opcode op, int pos) {
        if (scopeIndex >= scopes.size()) return;
        auto& scope = scopes[scopeIndex];
        scope.previousInstruction = scope.lastInstruction;
        scope.lastInstruction = {op, pos};
    }


#endif // COMPILER_H
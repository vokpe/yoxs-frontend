#ifndef COMPILER_H
#define COMPILER_H

#include "../ast/ast.hpp"
#include "../code/code.hpp"
#include "../object/object.hpp"

class CompilationScope {
public:
    code::Instructions instructions;
    EmittedInstruction lastInstruction;
    EmittedInstruction previousInstruction;
};

class Compiler {
private:
    std::vector<std::shared_ptr<object::Object>> constants;
    std::unique_ptr<SymbolTable> symbolTable;
    std::vector<CompilationScope> scopes;
    int scopeIndex;

public:
    Compiler();
    explicit Compiler(SymbolTable* s, const std::vector<std::shared_ptr<object::Object>>& consts);
    ~Compiler();

    void Compile(const ast::Node& node); // Replace ast::Node with the correct namespace or remove if unnecessary
    Bytecode Bytecode(); // Assuming Bytecode is a struct or class that needs to be defined

    // Other member functions as necessary...
};

#endif // COMPILER_H
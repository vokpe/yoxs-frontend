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

class Bytecode {
public:
    code::Instructions instructions;
    std::vector<std::shared_ptr<object::Object>> constants;

    // Constructors, destructors, and other member functions as necessary...
};

// Additional class or struct definitions as needed...


#endif // COMPILER_H
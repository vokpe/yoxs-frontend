#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include <string>
#include <unordered_map>
#include <vector>

enum class SymbolScope {
    Local,
    Global,
    Builtin,
    Free,
    Function
};

struct Symbol {
    std::string name;
    SymbolScope scope;
    int index;

    Symbol(const std::string& name = "", SymbolScope scope = SymbolScope::Global, int index = 0)
        : name(name), scope(scope), index(index) {}
};

class SymbolTable {
    SymbolTable* outer;
    std::unordered_map<std::string, Symbol> store;
    int numDefinitions;
    std::vector<Symbol> freeSymbols;

public:
    explicit SymbolTable(SymbolTable* outer = nullptr) : outer(outer), numDefinitions(0) {}

    static SymbolTable* NewEnclosedSymbolTable(SymbolTable* outer) {
        return new SymbolTable(outer);
    }

    Symbol Define(const std::string& name) {
        SymbolScope scope = (outer == nullptr) ? SymbolScope::Global : SymbolScope::Local;
        Symbol symbol{name, scope, numDefinitions++};
        store[name] = symbol;
        return symbol;
    }

    Symbol DefineBuiltin(int index, const std::string& name) {
        Symbol symbol{name, SymbolScope::Builtin, index};
        store[name] = symbol;
        return symbol;
    }

    Symbol DefineFunctionName(const std::string& name) {
        Symbol symbol{name, SymbolScope::Function, 0};
        store[name] = symbol;
        return symbol;
    }

    std::pair<Symbol, bool> Resolve(const std::string& name) {
        auto it = store.find(name);
        if (it != store.end()) {
            return {it->second, true};
        } else if (outer != nullptr) {
            auto resolved = outer->Resolve(name);
            if (!resolved.second) {
                return resolved;
            }

            if (resolved.first.scope == SymbolScope::Global || resolved.first.scope == SymbolScope::Builtin) {
                return resolved;
            }

            return {defineFree(resolved.first), true};
        }
        return {Symbol(), false};
    }

    const std::vector<Symbol>& GetFreeSymbols() const {
        return freeSymbols;
    }

private:
    Symbol defineFree(const Symbol& original) {
        freeSymbols.push_back(original);
        Symbol symbol{original.name, SymbolScope::Free, static_cast<int>(freeSymbols.size() - 1)};
        store[original.name] = symbol;
        return symbol;
    }
};

#endif // SYMBOL_TABLE_HPP

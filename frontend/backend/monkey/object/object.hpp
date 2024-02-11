// object.hpp
#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <memory>
#include <vector>
#include <sstream>
#include <functional>
#include <map>
#include "../ast/ast.hpp"

namespace YOXS_OBJECT {

class Environment;  // Forward declaration
class Object;

using BuiltinFunction = std::function<std::shared_ptr<Object>(const std::vector<std::shared_ptr<Object>>& args)>;

enum ObjectType {
    NULL_OBJ,
    ERROR_OBJ,

    INTEGER_OBJ,
    BOOLEAN_OBJ,
    STRING_OBJ,

    RETURN_VALUE_OBJ,

    FUNCTION_OBJ,
    BUILTIN_OBJ,

    ARRAY_OBJ,
    HASH_OBJ
};

std::string ObjectTypeToString(ObjectType type);

class HashKey {
public:
    ObjectType Type;
    int64_t Value;
    HashKey(const ObjectType& t, const int64_t& v) : Type(t), Value(v) {}

    bool operator ==(const HashKey& rhs) const {
        return (ObjectTypeToString(this->Type) == ObjectTypeToString(rhs.Type)) && (this->Value == rhs.Value);
    }

    bool operator !=(const HashKey& rhs) const {
        return !(*this == rhs);
    }

    bool operator < (const HashKey& rhs) const {
        if (Type != rhs.Type) {
            return Type < rhs.Type;
        }
        return Value < rhs.Value;
    }

    bool operator > (const HashKey& rhs) const {
        if (Type != rhs.Type) {
            return Type > rhs.Type;
        }
        return Value > rhs.Value;
    }

};

class Hashable {
public:
    virtual ~Hashable() = default;
    virtual HashKey keyHash() const = 0;

};

class Object {
public:
    virtual ~Object() = default; // Virtual destructor
    virtual ObjectType Type() const = 0;
    virtual std::string Inspect() const = 0;
};

class Integer : public Object, public Hashable {
public:
    int64_t Value;

    Integer(int64_t value) : Value(value) {}
    ObjectType Type() const override { return INTEGER_OBJ; }
    std::string Inspect() const override { return std::to_string(Value); }
    HashKey keyHash() const override {
        return {INTEGER_OBJ, Value};
    }
};

class BooleanObject : public Object, public Hashable {
public:
    bool Value;

    BooleanObject(bool value) : Value(value) {}
    ObjectType Type() const override { return BOOLEAN_OBJ; }
    std::string Inspect() const override { return Value ? "true" : "false"; }
    HashKey keyHash() const override { return {this->Type(), Value ? 1 : 0}; }
};

class NullObject : public Object {
public:
    ObjectType Type() const override { return NULL_OBJ; }
    std::string Inspect() const override { return "null"; }
};

class ReturnValue : public Object {
public:
    std::shared_ptr<Object> Value;

    ReturnValue(std::shared_ptr<Object> value) : Value(value) {}
    ObjectType Type() const override { return RETURN_VALUE_OBJ; }
    std::string Inspect() const override { return Value->Inspect(); }
};

class Error : public Object {
public:
    std::string Message;

    Error(const std::string& message) : Message(message) {}
    ObjectType Type() const override { return ERROR_OBJ; }
    std::string Inspect() const override { return "ERROR: " + Message; }
};

class Function : public Object {
public:
    std::vector<std::shared_ptr<YOXS_AST::Identifier>> Parameters;
    std::shared_ptr<Environment> Env;
    std::shared_ptr<YOXS_AST::BlockStatement> Body;

    Function(const std::vector<std::shared_ptr<YOXS_AST::Identifier>>& parameters, std::shared_ptr<Environment> env, std::shared_ptr<YOXS_AST::BlockStatement> body)
        : Parameters(parameters), Env(env), Body(body) {}
    ObjectType Type() const override { return FUNCTION_OBJ; }
    std::string Inspect() const override;
};

class String : public Object, public Hashable {
public:
    std::string Value;
    String(const std::string& val) : Value(val) {}
    ObjectType Type() const override { return STRING_OBJ; }
    std::string Inspect() const override { return Value; }
    HashKey keyHash() const override {
        std::hash<std::string> hasher;
        return {STRING_OBJ, static_cast<int64_t>(hasher(Value))};
    }

};

class Builtin : public Object {
public:
    BuiltinFunction function;
    Builtin(BuiltinFunction fn) : function(fn) {}

    ObjectType Type() const override { return BUILTIN_OBJ; }
    std::string Inspect() const override { return "builtin function"; }
};

class ArrayObject : public Object {
public: 
    std::vector<std::shared_ptr<Object>> Elements;
    ArrayObject(const std::vector<std::shared_ptr<Object>>& elms) : Elements(elms) {}
    ObjectType Type() const override { return ARRAY_OBJ; }
    std::string Inspect() const override {
        std::ostringstream out;

        std::vector<std::string> elements; 
        for(const auto& e : Elements){
            elements.push_back(e->Inspect());
        }
 
        out << "[" << YOXS_AST::join(elements, ", ") << "]";
        return out.str();
    }
};

class HashPair {
public:
    std::shared_ptr<Object> Key;
    std::shared_ptr<Object> Value;
};

class Hash : public Object {
public:
    Hash(const std::map<HashKey, HashPair>& p) : Pairs(p) {}
    std::map<HashKey, HashPair> Pairs;
    ObjectType Type() const override { return HASH_OBJ; }
    std::string Inspect() const override {
        std::ostringstream out; 
        
        std::vector<std::string> pairs; 
        for(const auto& pair : Pairs){
            pairs.push_back(ObjectTypeToString(pair.first.Type) + ":" + pair.second.Value->Inspect());
        }

        out << "{" << YOXS_AST::join(pairs, ", ") << "}";
        return out.str();
    }
};


} //namespace of YOXS_OBJECT

#include "environment.hpp" //have to declare it down here cuz it mess shit up when both headers rely on each other

#endif // OBJECT_H
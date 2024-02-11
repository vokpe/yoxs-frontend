#include "evaluator.hpp"

//evaluator.cpp

std::shared_ptr<NullObject> ObjectConstants::NULL_OBJ = std::make_shared<NullObject>();
std::shared_ptr<BooleanObject> ObjectConstants::TRUE = std::make_shared<BooleanObject>(true);
std::shared_ptr<BooleanObject> ObjectConstants::FALSE = std::make_shared<BooleanObject>(false);

std::map<std::string, std::shared_ptr<Builtin>> builtins = {
    {"len", std::make_shared<Builtin>([](const std::vector<std::shared_ptr<Object>>& args) -> std::shared_ptr<Object> {
        if (args.size() != 1) {
            return Evaluator::newError("wrong number of arguments. got=%zu, want=1", args.size());
        }

        auto argType = args[0]->Type();
        if (argType == ARRAY_OBJ) {
            auto arrayObj = std::dynamic_pointer_cast<ArrayObject>(args[0]);
            return std::make_shared<Integer>(arrayObj->Elements.size());
        } else if (argType == STRING_OBJ) {
            auto stringObj = std::dynamic_pointer_cast<String>(args[0]);
            return std::make_shared<Integer>(stringObj->Value.size());
        } else {
            return Evaluator::newError("argument to `len` not supported, got %s", ObjectTypeToString(argType).c_str());
        }
    })},
    {"puts", std::make_shared<Builtin>([](const std::vector<std::shared_ptr<Object>>& args) -> std::shared_ptr<Object> {
        for (auto& arg : args) {
            std::cout << arg->Inspect() << std::endl;
        }
        return ObjectConstants::NULL_OBJ;
    })},

    {"first", std::make_shared<Builtin>([](const std::vector<std::shared_ptr<Object>>& args) -> std::shared_ptr<Object> {
        if (args.size() != 1) {
            return Evaluator::newError("wrong number of arguments. got=" + std::to_string(args.size()) + ", want=1");
        }
        if (args[0]->Type() != ARRAY_OBJ) {
            return Evaluator::newError("argument to `first` must be ARRAY, got " + ObjectTypeToString(args[0]->Type()));
        }
        auto arr = std::dynamic_pointer_cast<ArrayObject>(args[0]);
        if (!arr->Elements.empty()) {
            return arr->Elements.front();
        }
        return ObjectConstants::NULL_OBJ;
    })},

    {"last", std::make_shared<Builtin>([](const std::vector<std::shared_ptr<Object>>& args) -> std::shared_ptr<Object> {
        if (args.size() != 1) {
            return Evaluator::newError("wrong number of arguments. got=" + std::to_string(args.size()) + ", want=1");
        }
        if (args[0]->Type() != ARRAY_OBJ) {
            return Evaluator::newError("argument to `last` must be ARRAY, got " + ObjectTypeToString(args[0]->Type()));
        }
        auto arr = std::dynamic_pointer_cast<ArrayObject>(args[0]);
        if (!arr->Elements.empty()) {
            return arr->Elements.back();
        }
        return ObjectConstants::NULL_OBJ;
    })},

    {"rest", std::make_shared<Builtin>([](const std::vector<std::shared_ptr<Object>>& args) -> std::shared_ptr<Object> {
        if (args.size() != 1) {
            return Evaluator::newError("wrong number of arguments. got=" + std::to_string(args.size()) + ", want=1");
        }
        if (args[0]->Type() != ARRAY_OBJ) {
            return Evaluator::newError("argument to `rest` must be ARRAY, got " + ObjectTypeToString(args[0]->Type()));
        }
        auto arr = std::dynamic_pointer_cast<ArrayObject>(args[0]);
        if (arr->Elements.size() > 1) {
            std::vector<std::shared_ptr<Object>> newElements(arr->Elements.begin() + 1, arr->Elements.end());
            return std::make_shared<ArrayObject>(newElements);
        }
        return ObjectConstants::NULL_OBJ;
    })},

    {"push", std::make_shared<Builtin>([](const std::vector<std::shared_ptr<Object>>& args) -> std::shared_ptr<Object> {
        if (args.size() != 2) {
            return Evaluator::newError("wrong number of arguments. got=" + std::to_string(args.size()) + ", want=2");
        }
        if (args[0]->Type() != ARRAY_OBJ) {
            return Evaluator::newError("argument to `push` must be ARRAY, got " + ObjectTypeToString(args[0]->Type()));
        }
        auto arr = std::dynamic_pointer_cast<ArrayObject>(args[0]);
        auto newElements = arr->Elements;
        newElements.push_back(args[1]);
        return std::make_shared<ArrayObject>(newElements);
    })}
};

std::shared_ptr<Object> Evaluator::Eval(std::shared_ptr<Node> node, std::shared_ptr<Environment> env) {
    // The dynamic_cast will check the actual type of Node and return nullptr if the cast is not valid.
    if (auto n = std::dynamic_pointer_cast<Program>(node)) {
        return evalProgram(n, env);
    } else if (auto n = std::dynamic_pointer_cast<BlockStatement>(node)) {
        return evalBlockStatement(n, env);
    } else if (auto n = std::dynamic_pointer_cast<ExpressionStatement>(node)) {
        return Eval(n->expr, env);
    } else if (auto n = std::dynamic_pointer_cast<ReturnStatement>(node)) {
        auto val = Eval(n->ReturnValue, env);
        if (isError(val)) {
            return val;
        }
        return std::make_shared<ReturnValue>(val);
    } else if (auto n = std::dynamic_pointer_cast<LetStatement>(node)){
        auto val = Eval(n->Value, env);
        if(Evaluator::isError(val)) {
            return val;
        }
        env->Set(n->Name->Value(), val);
    } else if (auto n = std::dynamic_pointer_cast<IntegerLiteral>(node)){
        return std::make_shared<Integer>(n->Value);
    } else if (auto n = std::dynamic_pointer_cast<StringLiteral>(node)){
        return std::make_shared<String>(n->Value);
    } else if (auto n = std::dynamic_pointer_cast<Boolean>(node)){
        return nativeBoolToBooleanObject(n->Value);
    } else if (auto n = std::dynamic_pointer_cast<PrefixExpression>(node)){
        auto right = Eval(n->Right, env);
        if(isError(right)) {
            return right;
        }
        return evalPrefixExpression(n->Operator, right);
    } else if (auto n = std::dynamic_pointer_cast<InfixExpression>(node)){
        auto left = Eval(n->Left, env);
        if(isError(left)){
            return left;
        }

        auto right = Eval(n->Right, env);
        if(isError(right)) {
            return right;
        }
        return evalInfixExpression(n->Operator, left, right);
    } else if (auto n = std::dynamic_pointer_cast<IfExpression>(node)){
        return evalIfExpression(n, env);
    } else if (auto n = std::dynamic_pointer_cast<Identifier>(node)){
        return evalIdentifier(n, env);
    } else if (auto n = std::dynamic_pointer_cast<FunctionLiteral>(node)){
        auto params = n->Parameters;
        auto body = n->Body;
        return std::make_shared<Function>(params, env, body);
    } else if (auto n = std::dynamic_pointer_cast<CallExpression>(node)){
        auto function = Eval(n->Function, env);
        
        if(isError(function)){
            return function;
        }

        auto args = evalExpressions(n->Arguments, env);
        if(args.size() == 1 && isError(args[0])){
            return args[0];
        }
        return applyFunction(function, args);
    } else if (auto n = std::dynamic_pointer_cast<ArrayLiteral>(node)){
        auto elements = evalExpressions(n->Elements, env);
        if(elements.size() == 1 && isError(elements[0])) return elements[0];
        return std::make_shared<ArrayObject>(elements);
    } else if (auto n = std::dynamic_pointer_cast<IndexExpression>(node)) {
        auto left = Eval(n->Left, env);
        if(isError(left)) return left;
        auto index = Eval(n->Index, env);
        return evalIndexExpression(left, index);
    } else if (auto n = std::dynamic_pointer_cast<HashLiteral>(node)){
        return evalHashLiteral(n, env);
    }

    return nullptr;
}

std::shared_ptr<Object> Evaluator::evalProgram(std::shared_ptr<Program> program, std::shared_ptr<Environment> env){
    std::shared_ptr<Object> result;

    for(auto& stmt : program->Statements){
        result = Eval(stmt, env);
        if(auto returnValue = std::dynamic_pointer_cast<ReturnValue>(result)){
            return returnValue->Value;
        }
        else if(auto errorValue = std::dynamic_pointer_cast<Error>(result)){
            return errorValue;
        }
    }

    return result;
}

std::shared_ptr<Object> Evaluator::evalBlockStatement(std::shared_ptr<BlockStatement> block, std::shared_ptr<Environment> env){
    std::shared_ptr<Object> result;

    for(auto& stmt: block->Statements) {
        result = Eval(stmt, env);
        if(result){
            auto rt = result->Type();
            if(rt == RETURN_VALUE_OBJ or rt == ERROR_OBJ){
                return result;
            }
        }
    }

    return result;
}

std::shared_ptr<BooleanObject> Evaluator::nativeBoolToBooleanObject(bool input){
    return input ? ObjectConstants::TRUE : ObjectConstants::FALSE;
}

std::shared_ptr<Object> Evaluator::evalPrefixExpression(const std::string& op, std::shared_ptr<Object> right){
    if(op == "!"){
        return evalBangOperatorExpression(right);
    }
    else if(op == "-"){
        return evalMinusPrefixOperatorExpression(right);
    }
    else{
        return newError("unknown operator: %s%s", op.c_str(), ObjectTypeToString(right->Type()).c_str());
    }
}

std::shared_ptr<Object> Evaluator::evalInfixExpression(const std::string& op, std::shared_ptr<Object> left, std::shared_ptr<Object> right){
    if (left->Type() != right->Type()) {
        return newError("type mismatch: %s %s %s", ObjectTypeToString(left->Type()).c_str(), op.c_str(), ObjectTypeToString(right->Type()).c_str());
    } else if (left->Type() == INTEGER_OBJ && right->Type() == INTEGER_OBJ) {
        return evalIntegerInfixExpression(op, left, right);
    } else if(left->Type() == STRING_OBJ && right->Type() == STRING_OBJ) {
        return evalStringInfixExpression(op, left, right);
    } else if (op == "==") {
        return nativeBoolToBooleanObject(left == right);
    } else if (op == "!=") {
        return nativeBoolToBooleanObject(left != right);
    } else {
        return newError("unknown operator: %s %s %s", ObjectTypeToString(left->Type()).c_str(), op.c_str(), ObjectTypeToString(right->Type()).c_str());
    }
}

std::shared_ptr<Object> Evaluator::evalBangOperatorExpression(std::shared_ptr<Object> right){
    if(right == ObjectConstants::TRUE){
        return ObjectConstants::FALSE;
    }
    else if(right == ObjectConstants::FALSE){
        return ObjectConstants::TRUE;
    }
    else if(right == ObjectConstants::NULL_OBJ){
        return ObjectConstants::TRUE;
    }
    else{
        return ObjectConstants::FALSE;
    }
}

std::shared_ptr<Object> Evaluator::evalMinusPrefixOperatorExpression(std::shared_ptr<Object> right){
    if(right->Type() != INTEGER_OBJ){
        return newError("unknown operator: -%s", ObjectTypeToString(right->Type()).c_str());
    }

    int value = std::static_pointer_cast<Integer>(right)->Value;
    return std::make_shared<Integer>(-value);
}

std::shared_ptr<Object> Evaluator::evalIntegerInfixExpression(const std::string& op, std::shared_ptr<Object> left, std::shared_ptr<Object> right){
    int leftVal = std::static_pointer_cast<Integer>(left)->Value;
    int rightVal = std::static_pointer_cast<Integer>(right)->Value;

    if(op == "+") { return std::make_shared<Integer>(leftVal + rightVal);}
    else if (op == "-") { return std::make_shared<Integer>(leftVal - rightVal); }
    else if (op == "*") { return std::make_shared<Integer>(leftVal * rightVal); }
    else if (op == "/") { return std::make_shared<Integer>(leftVal / rightVal); }
    else if (op == "<") { return nativeBoolToBooleanObject(leftVal < rightVal); }
    else if (op == ">") { return nativeBoolToBooleanObject(leftVal > rightVal); }
    else if (op == "==") { return nativeBoolToBooleanObject(leftVal == rightVal); }
    else if (op == "!=") { return nativeBoolToBooleanObject(leftVal != rightVal); }
    else {return newError("unknown operator: %s %s %s", ObjectTypeToString(left->Type()).c_str(), op.c_str(), ObjectTypeToString(right->Type()).c_str()); }
    //else {return newError("unknown operator: %s %s %s", left->Inspect().c_str(), op.c_str(), right->Inspect().c_str()); }
}

std::shared_ptr<Object> Evaluator::evalStringInfixExpression(const std::string& op, std::shared_ptr<Object> left, std::shared_ptr<Object> right){
    if(op != "+"){
       return newError("unknown operator: %s %s %s", ObjectTypeToString(left->Type()).c_str(), op.c_str(), ObjectTypeToString(right->Type()).c_str());
    }
    std::string leftVal = std::static_pointer_cast<String>(left)->Value;
    std::string rightVal = std::static_pointer_cast<String>(right)->Value;

    return std::make_shared<String>(leftVal + rightVal);
}

std::shared_ptr<Object> Evaluator::evalIfExpression(std::shared_ptr<IfExpression> ie, std::shared_ptr<Environment> env){
    auto condition = Eval(ie->Condition, env);
    if(isError(condition)) return condition;
    if(isTruthy(condition)){
        return Eval(ie->Consequence, env);
    }
    else if(ie->Alternative){
        return Eval(ie->Alternative, env);
    }
    else{
        return ObjectConstants::NULL_OBJ;
    }
}

std::shared_ptr<Object> Evaluator::evalIdentifier(std::shared_ptr<Identifier> node, std::shared_ptr<Environment> env){
    auto val = env->Get(node->Value());
    if (val) {
        return val;
    }

    // If not found in the environment, check if it's a built-in function
    auto it = builtins.find(node->Value());
    if (it != builtins.end()) {
        return it->second;  // Return the built-in function
    }

    // If neither in environment nor a built-in, return an error
    return newError("identifier not found: " + node->Value());
}

bool Evaluator::isTruthy(std::shared_ptr<Object> obj){
    if(obj == ObjectConstants::NULL_OBJ) return false;
    else if(obj == ObjectConstants::TRUE) return true;
    else if(obj == ObjectConstants::FALSE) return false;
    else return true;
}

//The ellipsis ... in the parameter list of a function, like the 
//newError function in Go or C++, is used to denote a variadic 
//function. This means that the function can accept an arbitrary 
//number of arguments of a specified type. 
std::shared_ptr<Error> Evaluator::newError(const std::string format, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format.c_str(), args);
    va_end(args);
    return std::make_shared<Error>(buffer);
}


bool Evaluator::isError(std::shared_ptr<Object> obj){
    if(obj) return obj->Type() == ERROR_OBJ;
    return false;
}

std::vector<std::shared_ptr<Object>> Evaluator::evalExpressions(std::vector<std::shared_ptr<Expression>> exps, std::shared_ptr<Environment> env){
    std::vector<std::shared_ptr<Object>> result;
    for (auto& exp : exps) {
        auto evaluated = Eval(exp, env);
        if (isError(evaluated)) {
            // If an error occurs, return a vector with just that error.
            return {evaluated};
        }
        result.push_back(evaluated);
    }
    return result;
}

std::shared_ptr<Object> Evaluator::applyFunction(std::shared_ptr<Object> fn, std::vector<std::shared_ptr<Object>> args){
    if(auto fnCast = std::dynamic_pointer_cast<Function>(fn)){
        auto extendedEnv = extendFunctionEnv(fnCast, args);
        auto evaluated = Eval(fnCast->Body, extendedEnv);
        return unwrapReturnValue(evaluated);
    } else if (auto fnCast = std::dynamic_pointer_cast<Builtin>(fn)){
        return fnCast->function(args);
    }
    //else
    return newError("not a function: %s", fn->Inspect().c_str());
}

std::shared_ptr<Environment> Evaluator::extendFunctionEnv(std::shared_ptr<Function> fn, std::vector<std::shared_ptr<Object>> args){
    auto env = std::make_shared<Environment>(fn->Env);
    for (size_t i = 0; i < fn->Parameters.size(); ++i) {
        env->Set(fn->Parameters[i]->Value(), args[i]);
    }
    return env;
}

std::shared_ptr<Object> Evaluator::unwrapReturnValue(std::shared_ptr<Object> obj){
    auto returnValue = std::dynamic_pointer_cast<ReturnValue>(obj);
    if (returnValue) {
        return returnValue->Value;
    }
    return obj;
}

std::shared_ptr<Object> Evaluator::evalIndexExpression(std::shared_ptr<Object> left, std::shared_ptr<Object> index){
    if(left->Type() == ARRAY_OBJ && index->Type() == INTEGER_OBJ) return evalArrayIndexExpression(left, index);
    else if(left->Type() == HASH_OBJ) return evalHashIndexExpression(left, index);
    else {return newError("index operator not supported: %s", ObjectTypeToString(left->Type()).c_str()); }
}

std::shared_ptr<Object> Evaluator::evalArrayIndexExpression(std::shared_ptr<Object> array, std::shared_ptr<Object> index){
    auto arrayObject = std::dynamic_pointer_cast<ArrayObject>(array);
    int idx = std::dynamic_pointer_cast<Integer>(index)->Value;
    int max = arrayObject->Elements.size() - 1;

    if(idx < 0 or idx > max) return ObjectConstants::NULL_OBJ;

    return arrayObject->Elements[idx];
}

std::shared_ptr<Object> Evaluator::evalHashLiteral(std::shared_ptr<HashLiteral> node, std::shared_ptr<Environment> env){
    std::map<HashKey, HashPair> pairs;
    for(const auto& nodePair : node->Pairs) {
        auto key = Eval(nodePair.first, env);
        if(isError(key)) return key;

        auto hashKey = std::dynamic_pointer_cast<Hashable>(key);
        if(!hashKey) return newError("unusable as hash key: %s", ObjectTypeToString(key->Type()).c_str());

        auto value = Eval(nodePair.second, env);
        if(isError(key)) return value;

        auto hashed = hashKey->keyHash();
        pairs[hashed] = HashPair{key, value};
    }

    return std::make_shared<Hash>(pairs);
}

std::shared_ptr<Object> Evaluator::evalHashIndexExpression(std::shared_ptr<Object> hash, std::shared_ptr<Object> index){
    auto hashObject = std::dynamic_pointer_cast<Hash>(hash);

    auto key = std::dynamic_pointer_cast<Hashable>(index);
    if(!key) return newError("unusable as hash key: %s", ObjectTypeToString(index->Type()).c_str());
    auto pair = hashObject->Pairs.find(key->keyHash());
    if(pair == hashObject->Pairs.end()) return ObjectConstants::NULL_OBJ;

    return pair->second.Value;
}

//g++ -std=c++17 -Isrc -c src/monkey/evaluator/evaluator.cpp -o evaluator.o
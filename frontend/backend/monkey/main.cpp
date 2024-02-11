#include "repl/repl.hpp"
#include <iostream>

int main() {
    std::cout << "This is the Monkey programming language!" << std::endl;
    std::cout << "Feel free to type in commands" << std::endl;

    // Start the REPL using the standard input and output.
    //REPL::Start(std::cin, std::cout);
    REPL::StartSingle(std::cin, std::cout);

    return 0;
}
//g++ -std=c++17 -I. -o monkey_repl main.cpp repl/repl.cpp object/object.cpp object/environment.cpp lexer/lexer.cpp parser/parser.cpp evaluator/evaluator.cpp ast/ast.cpp token/token.cpp && ./monkey_repl


/*
Feel free to type in commands
>> let x = 1 * 2 * 3 * 4 * 5
let x = ((((1 * 2) * 3) * 4) * 5);
>> x * y / 2 + 3 * 8 - 123
((((x * y) / 2) + (3 * 8)) - 123)
>> true == false
(true == false)
>>
*/

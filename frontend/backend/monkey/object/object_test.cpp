#include "../lexer/lexer.hpp"
#include "object.hpp"
#include "environment.hpp"
#include "../parser/parser.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <cassert>
#include <variant>

void TestStringHashKey() {
    YOXS_OBJECT::String hello1("Hello World");
    YOXS_OBJECT::String hello2("Hello World");
    YOXS_OBJECT::String diff1("My name is johnny");
    YOXS_OBJECT::String diff2("My name is johnny");

    if(hello1.keyHash() != hello2.keyHash()){
        std::cerr << "strings with same content have different hash keys\n";
    }
    if(diff1.keyHash() != diff2.keyHash()){
        std::cerr << "strings with same content have different hash keys\n";
    }
    if(hello1.keyHash() == diff1.keyHash()){
        std::cerr << "strings with different content have same hash keys\n";
    }
}

void TestBooleanHashKey() {
    YOXS_OBJECT::BooleanObject true1(true);
    YOXS_OBJECT::BooleanObject true2(true);
    YOXS_OBJECT::BooleanObject false1(false);
    YOXS_OBJECT::BooleanObject false2(false);

    if (true1.keyHash() != true2.keyHash()) {
		std::cerr << "trues do not have same hash key\n";
	}

	if (false1.keyHash() != false2.keyHash()) {
		std::cerr << "falses do not have same hash key\n";
	}

	if (true1.keyHash() == false1.keyHash()) {
		std::cerr << "true has same hash key as false\n";
	}
}

void TestIntegerHashKey() {
    YOXS_OBJECT::Integer one1(1);
    YOXS_OBJECT::Integer one2(1);
    YOXS_OBJECT::Integer two1(2);
    YOXS_OBJECT::Integer two2(2);

    if (one1.keyHash() != one2.keyHash()) {
		std::cerr << "integers with same content have two different hash keys\n";
	}

	if (two1.keyHash() != two2.keyHash()) {
		std::cerr << "integers with same content have two different hash keys\n";
	}

	if (one1.keyHash() == two1.keyHash()) {
		std::cerr << "integers with twoerent content have same hash keys\n";
	}
}

int main() {
    TestStringHashKey();
    TestIntegerHashKey();
    TestIntegerHashKey();
    std::cout << "object tests have finished!\n";
}
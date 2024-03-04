#include "code.hpp"
#include <sstream>
#include <cassert>
#include <iostream>

void TestMake();
void TestInstructionsString();
void TestReadOperands();


int main() {
    TestMake();
    TestInstructionsString();
    TestReadOperands();

    std::cout << "All tests passed successfully!" << std::endl;
    return 0;
}

void TestMake(){
    struct TestCase {
        Opcode op;
        std::vector<int> operands;
        std::vector<uint8_t> expected;
    };

     std::vector<TestCase> tests = {
        {Opcode::OpConstant, {65534}, {static_cast<uint8_t>(Opcode::OpConstant), 255, 254}},
        {Opcode::OpAdd, {}, {static_cast<uint8_t>(Opcode::OpAdd)}},
        {Opcode::OpGetLocal, {255}, {static_cast<uint8_t>(Opcode::OpGetLocal), 255}},
        {Opcode::OpClosure, {65534, 255}, {static_cast<uint8_t>(Opcode::OpClosure), 255, 254, 255}},
    };

    for (const auto& tt : tests) {
        auto instruction = Make(tt.op, tt.operands);
        assert(instruction == tt.expected && "Instruction or operands do not match expected output.");
    }
}

void TestInstructionsString() {
    std::vector<std::vector<uint8_t>> instructions = {
        Make(Opcode::OpAdd),
        Make(Opcode::OpGetLocal, {1}),
        Make(Opcode::OpConstant, {2}),
        Make(Opcode::OpConstant, {65535}),
        Make(Opcode::OpClosure, {65535, 255}),
    };

    std::string expected = 
        "0000 OpAdd\n"
        "0001 OpGetLocal 1\n"
        "0003 OpConstant 2\n"
        "0006 OpConstant 65535\n"
        "0009 OpClosure 65535 255\n";

    std::vector<uint8_t> concatted;
    for (auto& ins : instructions) {
        concatted.insert(concatted.end(), ins.begin(), ins.end());
    }

    std::cout << "Actual: " << InstructionsToString(concatted) << std::endl;
    std::cout << "Expected: " << expected << std::endl;

    assert(InstructionsToString(concatted) == expected && "Instructions wrongly formatted.");
}

void TestReadOperands() {
    struct TestCase {
        Opcode op;
        std::vector<int> operands;
        int bytesRead;
    };

    std::vector<TestCase> tests = {
        {Opcode::OpConstant, {65535}, 2},
        {Opcode::OpGetLocal, {255}, 1},
        {Opcode::OpClosure, {65535, 255}, 3},
    };

    for (const auto& tt : tests) {
        auto instruction = Make(tt.op, tt.operands);
        auto def = Lookup(tt.op); // Assuming Lookup has been adapted to not use error return but maybe throw exceptions or assert on failure

        auto [operandsRead, n] = ReadOperands(def, instruction, 1);
        assert(n == tt.bytesRead && "Bytes read count mismatch.");

        for (size_t i = 0; i < tt.operands.size(); ++i) {
            assert(operandsRead[i] == tt.operands[i] && "Operand mismatch.");
        }
    }
}


//g++ -std=c++20 -Isrc -c code_test.cpp -o code_test.o && ./code_test
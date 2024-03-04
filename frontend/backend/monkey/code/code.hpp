#ifndef CODE_H
#define CODE_H

#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
#include <cstring> // For memcpy
#include <iomanip>

enum class Opcode {
    OpConstant,
    
	OpAdd,

	OpPop,

	OpSub,
	OpMul,
	OpDiv,

	OpTrue,
	OpFalse,

	OpEqual,
	OpNotEqual,
	OpGreaterThan,

	OpMinus,
	OpBang,

	OpJumpNotTruthy,
	OpJump,

	OpNull,

	OpGetGlobal,
	OpSetGlobal,

	OpArray,
	OpHash,
	OpIndex,

	OpCall,

	OpReturnValue,
	OpReturn,

	OpGetLocal,
	OpSetLocal,

	OpGetBuiltin,

	OpClosure,

	OpGetFree,

	OpCurrentClosure
};

class Definition {
public: 
    std::string Name;
    std::vector<int> OperandWidths;
    Definition(std::string _Name, std::vector<int> _OperandWidths);
};


//Map to store opcode definitions
extern std::unordered_map<Opcode, Definition> definitions;

std::string fmtInstruction(const Definition& def, const std::vector<int>& operands, size_t index);

std::string InstructionsToString(const std::vector<uint8_t>& instructions);

const Definition& Lookup(Opcode op);

// Helper function to create an Instructions object with opcode and operands
std::vector<uint8_t> Make(Opcode op, const std::vector<int>& operands = {});

std::pair<std::vector<int>, int> ReadOperands(const Definition& def, const std::vector<uint8_t>& ins, size_t offset);

uint8_t readUint8(const std::vector<uint8_t>& ins, size_t& offset);

int readUint16(const std::vector<uint8_t>& ins, size_t offset);

#endif // CODE_H
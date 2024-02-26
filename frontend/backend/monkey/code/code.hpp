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

	Definition(std::string _Name, std::vector<int> _OperandWidths) : Name(_Name), OperandWidths(_OperandWidths) {}

};

//Map to store opcode definitions
std::unordered_map<Opcode, Definition> definitions = {

	{Opcode::OpConstant, {"OpConstant", {2}}},
	{Opcode::OpAdd, {"OpAdd", {}}},
	{Opcode::OpPop, {"OpPop", {}}},
	{Opcode::OpSub, {"OpSub", {}}},
	{Opcode::OpMul, {"OpMul", {}}},
	{Opcode::OpDiv, {"OpDiv", {}}},
	{Opcode::OpTrue, {"OpTrue", {}}},
	{Opcode::OpFalse, {"OpFalse", {}}},
	{Opcode::OpEqual, {"OpEqual", {}}},
	{Opcode::OpNotEqual, {"OpNotEqual", {}}},
	{Opcode::OpGreaterThan, {"OpGreaterThan", {}}},
	{Opcode::OpMinus, {"OpMinus", {}}},
	{Opcode::OpBang, {"OpBang", {}}},
	{Opcode::OpJumpNotTruthy, {"OpJumpNotTruthy", {2}}},
	{Opcode::OpJump, {"OpJump", {2}}},
	{Opcode::OpNull, {"OpNull", {}}},
	{Opcode::OpGetGlobal, {"OpGetGlobal", {2}}},
	{Opcode::OpSetGlobal, {"OpSetGlobal", {2}}},
	{Opcode::OpArray, {"OpArray", {2}}},
	{Opcode::OpHash, {"OpHash", {2}}},
	{Opcode::OpIndex, {"OpIndex", {}}},
	{Opcode::OpCall, {"OpCall", {1}}},
	{Opcode::OpReturnValue, {"OpReturnValue", {}}},
	{Opcode::OpReturn, {"OpReturn", {}}},
	{Opcode::OpGetLocal, {"OpGetLocal", {1}}},
	{Opcode::OpSetLocal, {"OpSetLocal", {1}}},
	{Opcode::OpGetBuiltin, {"OpGetBuiltin", {1}}},
	{Opcode::OpClosure, {"OpClosure", {2, 1}}},
	{Opcode::OpGetFree, {"OpGetFree", {1}}},
	{Opcode::OpCurrentClosure, {"OpCurrentClosure", {}}},

};

class Instructions {
public: 
	std::vector<uint8_t> ins;
	Instructions(std::vector<uint8_t> _ins) : ins(_ins) {}

	std::string toString() {
		std::ostringstream out;
		for(size_t i = 0; i < ins.size();){
			auto op = static_cast<Opcode>(ins[i]);
			auto it = definitions.find(op);

			if(it == definitions.end()) {
				out << "ERROR: opcode " << static_cast<int>(ins[i]) << " undefined\n";
				i++;
				continue;
			}

			const auto& def = it->second;
			auto [operands, read] = readOperands(def, i + 1);

			out << fmtInstruction(def, operands, i) << "\n";
			i += 1 + read;
		}
		return out.str();
	}
private:
	std::pair<std::vector<int>, int> readOperands(const Definition& def, size_t offset){
		std::vector<int> operands;
		int totalRead = 0; 

		for(int width : def.OperandWidths) {
			if(width == 2){
				operands.push_back(readUint16(offset + totalRead));
				totalRead += 2;
			} else if (width == 1) {
				//readUint8
				operands.push_back(ins[offset + totalRead]);
				totalRead += 1;
			}
		}
		return {operands, totalRead};
	}

	int readUint16(size_t offset) {
        uint16_t value;
        memcpy(&value, ins.data() + offset, sizeof(uint16_t));
        return static_cast<int>(value);
    }

    std::string fmtInstruction(const Definition& def, const std::vector<int>& operands, size_t index) {
        std::ostringstream out;
        if (operands.size() != def.OperandWidths.size()) {
            out << "ERROR: operand len " << operands.size() << " does not match defined " << def.OperandWidths.size() << "\n";
            return out.str();
        }

        out << std::setw(4) << index << " " << def.Name;
        for (auto operand : operands) {
            out << " " << operand;
        }

        return out.str();
    }
};

// Helper function to create an Instructions object with opcode and operands
std::vector<uint8_t> Make(Opcode op, const std::vector<int>& operands = {}) {
    auto it = definitions.find(op);
    if (it == definitions.end()) {
        throw std::runtime_error("Opcode undefined");
    }

    std::vector<uint8_t> instruction;
    instruction.push_back(static_cast<uint8_t>(op));

    const auto& def = it->second;
    for (size_t i = 0; i < operands.size(); ++i) {
        int width = def.OperandWidths[i];
        if (width == 2) {
            uint16_t value = static_cast<uint16_t>(operands[i]);
            instruction.push_back(value >> 8);
            instruction.push_back(value & 0xFF);
        } else if (width == 1) {
            instruction.push_back(static_cast<uint8_t>(operands[i]));
        }
    }

    return instruction;
}
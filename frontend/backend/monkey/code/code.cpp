#include "code.hpp"


Definition::Definition(std::string _Name, std::vector<int> _OperandWidths) : Name(_Name), OperandWidths(_OperandWidths) {}

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

std::string fmtInstruction(const Definition& def, const std::vector<int>& operands, size_t index) {
	std::ostringstream out;
	if (operands.size() != def.OperandWidths.size()) {
		out << "ERROR: operand len " << operands.size() << " does not match defined " << def.OperandWidths.size() << "\n";
		return out.str();
	}

	out << std::setw(4) << std::setfill('0') << std::hex << index << std::dec << " " << def.Name;
	for (auto operand : operands) {
		out << " " << operand;
	}

	return out.str();
}

std::string InstructionsToString(const std::vector<uint8_t>& instructions) {
    std::ostringstream out;
    size_t i = 0;
    while (i < instructions.size()) {
        Opcode op = static_cast<Opcode>(instructions[i]);
        const auto& def = Lookup(op);
        auto [operands, read] = ReadOperands(def, instructions, i + 1);

        out << fmtInstruction(def, operands, i) << "\n";
        i += 1 + read;
    }
    return out.str();
}

const Definition& Lookup(Opcode op){
	auto it = definitions.find(op);
	if (it == definitions.end()) {
		throw std::runtime_error("Opcode undefined");
	}
	return it->second; // def
}

// Helper function to create an Instructions object with opcode and operands
std::vector<uint8_t> Make(Opcode op, const std::vector<int>& operands) {	const auto& def = Lookup(op);
	std::vector<uint8_t> instruction;
	
	instruction.push_back(static_cast<uint8_t>(op));

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

std::pair<std::vector<int>, int> ReadOperands(const Definition& def, const std::vector<uint8_t>& ins, size_t offset) {
    std::vector<int> operands;
    int totalRead = 0;

    for (int width : def.OperandWidths) {
        if (width == 2) {
            if (offset + totalRead + 1 >= ins.size()) throw std::runtime_error("Insufficient bytes for operand");
            uint16_t operand = (ins[offset + totalRead] << 8) | ins[offset + totalRead + 1];
            operands.push_back(operand);
            totalRead += 2;
        } else if (width == 1) {
            if (offset + totalRead >= ins.size()) throw std::runtime_error("Insufficient bytes for operand");
            operands.push_back(ins[offset + totalRead]);
            totalRead += 1;
        }
    }

    return {operands, totalRead};
}

uint8_t readUint8(const std::vector<uint8_t>& ins, size_t& offset) {
    uint8_t value = ins[offset];
    offset += 1; // Move past the byte that was read
    return value;
}

int readUint16(const std::vector<uint8_t>& ins, size_t offset) {
	uint16_t value;
	memcpy(&value, ins.data() + offset, sizeof(uint16_t));
	return static_cast<int>(value);
}
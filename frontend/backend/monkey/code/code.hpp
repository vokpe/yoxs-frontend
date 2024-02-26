const enum Opcode {
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
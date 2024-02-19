#include "../object/object.hpp"
#include "../object/environment.hpp"

using namespace YOXS_OBJECT;
using namespace YOXS_AST;

constexpr int StackSize = 2048;
constexpr int GlobalsSize = 65536;
constexpr int MaxFrames = 1024;

class ObjectConstants {
public:
    static std::shared_ptr<NullObject> NULL_OBJ;
    static std::shared_ptr<BooleanObject> TRUE;
    static std::shared_ptr<BooleanObject> FALSE;
};

std::shared_ptr<NullObject> ObjectConstants::NULL_OBJ = std::make_shared<NullObject>();
std::shared_ptr<BooleanObject> ObjectConstants::TRUE = std::make_shared<BooleanObject>(true);
std::shared_ptr<BooleanObject> ObjectConstants::FALSE = std::make_shared<BooleanObject>(false);

class VM {
public:
    VM() : sp(0), framesIndex(0) {
        stack.reserve(StackSize);
        globals.resize(GlobalsSize);
        frames.reserve(MaxFrames);
    }

    // Stack manipulation methods
    bool push(std::shared_ptr<Object> obj) {
        if (sp >= StackSize) {
            // Stack overflow handling
            return false;
        }
        stack.push_back(obj);
        sp++;
        return true;
    }

        std::shared_ptr<Object> pop() {
        if (sp == 0) {
            return nullptr; // Stack underflow handling
        }
        sp--;
        return stack[sp];
    }

    std::shared_ptr<Object> peek(int distance = 0) {
        if (sp - distance - 1 < 0) {
            return nullptr; // Invalid peek
        }
        return stack[sp - distance - 1];
    }

    // Frame management methods (simplified for demonstration)
    void pushFrame(int frame) {
        frames.push_back(frame);
        framesIndex++;
    }

    int popFrame() {
        if (framesIndex == 0) {
            return -1; // Call stack underflow handling
        }
        framesIndex--;
        return frames[framesIndex];
    }

    // Placeholder for bytecode execution method
    void executeBytecode() {
        // Implementation depends on bytecode format and instructions
    }
private:
    std::vector<std::shared_ptr<Object>> constants;
    std::vector<std::shared_ptr<Object>> stack;
    int sp; // stack pointer; always points to next value. 
    //top of stack is stack[sp-1]

    std::vector<std::shared_ptr<Object>> globals;
    std::vector<int> frames;
    int framesIndex;
};
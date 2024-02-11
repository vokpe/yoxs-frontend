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
private:
    std::vector<std::shared_ptr<Object>> constants;
    std::vector<std::shared_ptr<Object>> stack;
    int sp; // stack pointer; always points to next value. 
    //top of stack is stack[sp-1]

    std::vector<std::shared_ptr<Object>> globals;
    std::vector<int> frames;
    int framesIndex;
};
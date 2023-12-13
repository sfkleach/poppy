#include <cstdlib>
#include <vector>   
#include <fstream>
#include <iostream>
#include <ios>
#include <map>

#include "itemizer.hpp"
#include "itemrole.hpp"
#include "cell.hpp"
#include "heap.hpp"

#define DEBUG 1

namespace poppy {

enum class Instruction {
    PUSHS,
    PUSHQ,
    HALT
};

class Engine {
    friend class CodePlanter;
private:
    std::map<Instruction, void *> _opcode_map;
    std::vector<Cell> _valueStack;
    Heap _heap;

private:
    void init_or_run(Cell * pc, bool init) {

        // In order to get the address-of-labels into a map we need to
        // have a separate initialisation pass, so that the labels are
        // in scope while we populate the map.
        if (init) {
            _opcode_map = {
                {Instruction::PUSHS, &&L_PUSHS},
                {Instruction::PUSHQ, &&L_PUSHQ},
                {Instruction::HALT, &&L_HALT}
            };
            return;
        }

        //  Otherwise we are running, so we can jump straight to the
        //  appropriate address once we have confirmed we are indeed
        //  pointing to a procedure object.

        if (!pc->isProcedureKey()) {
            throw std::runtime_error("Not a procedure");
        }

        pc += 1; // Skip the procedure header (just the key for now).
        goto *pc->ref;

        L_PUSHQ:
            _valueStack.push_back(*pc++);
            goto *(pc++->ref);

        L_PUSHS:
            _valueStack.push_back(_valueStack.back());
            goto *(pc++->ref);

        L_HALT:
            if ( DEBUG ) std::cout << "DONE!" << std::endl;
            return;            
    }

public:
    void initialise() {
        init_or_run(nullptr, true);
    }

    void run(Cell * pc) {
        init_or_run(pc, false);
    }

};

class CodePlanter {
private:
    Engine & _engine;
    Builder _builder;

public:
    CodePlanter(Engine & engine) : 
        _engine(engine),
        _builder(engine._heap, Cell{ .u64 = Cell::ProcedureTag })
    {
    }
public:
    void addInstruction(Instruction inst) {
        Ref label_addr = _engine._opcode_map[inst];
        _builder.addCell(Cell{ .ref = label_addr });
    }

    Cell * procedure() {
        return _builder.object();
    }
};

int main(int argc, char **argv) {
    using namespace poppy;
    Engine engine;
    engine.initialise();
    
    std::ifstream source( "poem.txt" );
    source.unsetf(std::ios_base::skipws);
    Itemizer itemizer( source );
    Item item;
    while (itemizer.nextItem(item)) {
        std::cout << item.nameString() << std::endl;
        const char * k = itemCodeToItemKey(item.itemCode());
        std::cout << (k == nullptr ? "?" : k) << std::endl;
        std::cout << itemRoleToString(item.itemRole()) << std::endl;
    }

    CodePlanter planter(engine);
    planter.addInstruction(Instruction::HALT);
    Cell * pc = planter.procedure();

    engine.run(pc);

    return EXIT_SUCCESS;
}

} // namespace poppy



int main(int argc, char **argv) {
    poppy::main(argc, argv);
}

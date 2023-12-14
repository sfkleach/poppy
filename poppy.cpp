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
    ADD,
    HALT,
    POP_GLOBAL,
    PUSH_GLOBAL,
    PUSHQ,
    PUSHS,
    RETURN,
};



class Engine {
    friend class CodePlanter;
private:
    std::map<Instruction, void *> _opcode_map;
    Cell _exit_code[1];

private:
    std::vector<Cell> _valueStack;
    
    Cell * currentProcedure;
    std::vector<Cell> _callStack;

    std::map<std::string, RefIdent> _symbolTable;
    Heap _heap;

public:
    void declareGlobal(const std::string & name) {
        if (_symbolTable.find(name) != _symbolTable.end()) {
            std::cerr << "Redeclaring global: " << name << std::endl;
        }
        Ident * ident = new Ident(Cell::makeSmall(0));
        _symbolTable[name] = ident;
    }

private:
    void init_or_run(Cell * pc, bool init) {

        // In order to get the address-of-labels into a map we need to
        // have a separate initialisation pass, so that the labels are
        // in scope while we populate the map.
        if (init) {
            _opcode_map = {
                {Instruction::ADD, &&L_ADD},
                {Instruction::HALT, &&L_HALT},
                {Instruction::POP_GLOBAL, &&L_POP_GLOBAL},
                {Instruction::PUSH_GLOBAL, &&L_PUSH_GLOBAL},
                {Instruction::PUSHQ, &&L_PUSHQ},
                {Instruction::PUSHS, &&L_PUSHS},
                {Instruction::RETURN, &&L_RETURN},
            };
            _exit_code[0] = Cell{ .ref = &&L_HALT };
            return;
        }

        //  Otherwise we are running, so we can jump straight to the
        //  appropriate address once we have confirmed we are indeed
        //  pointing to a procedure object.

        if (!pc->isProcedureKey()) {
            throw std::runtime_error("Not a procedure");
        }

        currentProcedure = pc;
        _callStack.push_back( Cell{ .ref = nullptr } );
        _callStack.push_back( Cell{ .refCell = &_exit_code[0] } );
        pc += 1; // Skip the procedure header (just the key for now).
        goto *pc++->ref;

        L_POP_GLOBAL: {
            Cell c = _valueStack.back();
            _valueStack.pop_back();
            Ident * ident = (pc++)->refIdent;
            ident->value() = c;
            goto *(pc++->ref);
        }

        L_PUSH_GLOBAL: {
            Ident * ident = (pc++)->refIdent;
            _valueStack.push_back(ident->value());
            goto *(pc++->ref);
        }

        L_ADD: {
            int64_t a = _valueStack.back().i64;
            _valueStack.pop_back();
            _valueStack.back() = Cell{ .i64 = ( _valueStack.back().i64 + a ) };
            goto *(pc++->ref);
        }

        L_PUSHQ:
            _valueStack.push_back(*pc++);
            goto *(pc++->ref);

        L_PUSHS:
            _valueStack.push_back(_valueStack.back());
            goto *(pc++->ref);

        L_RETURN:
            pc = _callStack.back().refCell;
            _callStack.pop_back();
            currentProcedure = _callStack.back().refCell;
            _callStack.pop_back();
            goto *(pc++->ref);

        L_HALT:
            if ( DEBUG ) std::cout << "DONE!" << std::endl;
            return;            
    }

public:
    void initialise() {
        if (sizeof(Cell) != 8) {
            throw std::runtime_error("Cell is not 8 bytes");
        }
        init_or_run(nullptr, true);
    }

    void run(Cell * pc) {
        init_or_run(pc, false);
    }

public:
    void debugDisplay() {
        std::cout << "Dictionary" << std::endl;
        for (auto & [name, ident] : _symbolTable) {
            std::cout << name << " = " << ident->value().u64 << std::endl;
        }
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

    void addData(Cell cell) {
        _builder.addCell(cell);
    }

    Cell * procedure() {
        return _builder.object();
    }

public:
    void addGlobal(const std::string & name) {
        if (_engine._symbolTable.find(name) == _engine._symbolTable.end()) {
            std::cerr << "Global not declared: " << name << std::endl;
        }
        _builder.addCell(Cell::makeRefIdent(_engine._symbolTable[name]));
    }

    void declareGlobal(const std::string & name) {
        _engine.declareGlobal(name);
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
    planter.declareGlobal("x");
    planter.addInstruction(Instruction::PUSHQ);
    planter.addData(Cell::makeSmall(100));
    planter.addInstruction(Instruction::POP_GLOBAL);
    planter.addGlobal("x");
    planter.addInstruction(Instruction::PUSH_GLOBAL);
    planter.addGlobal("x");
    planter.addInstruction(Instruction::PUSHQ);
    planter.addData(Cell::makeSmall(1));
    planter.addInstruction(Instruction::ADD);
    planter.addInstruction(Instruction::POP_GLOBAL);
    planter.addGlobal("x");
    planter.addInstruction(Instruction::RETURN);
    Cell * pc = planter.procedure();

    engine.run(pc);

    engine.debugDisplay();

    return EXIT_SUCCESS;
}

} // namespace poppy



int main(int argc, char **argv) {
    poppy::main(argc, argv);
}

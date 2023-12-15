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
#include "mishap.hpp"
#include "layout.hpp"

#define DEBUG 1

namespace poppy {

enum class Instruction {
    ADD,
    HALT,
    MUL,
    PASSIGN,
    POP_GLOBAL,
    PUSH_GLOBAL,
    PUSHQ,
    PUSHS,
    RETURN,
    SUB,
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
                {Instruction::MUL, &&L_MUL},
                {Instruction::PASSIGN, &&L_PASSIGN},
                {Instruction::POP_GLOBAL, &&L_POP_GLOBAL},
                {Instruction::PUSH_GLOBAL, &&L_PUSH_GLOBAL},
                {Instruction::PUSHQ, &&L_PUSHQ},
                {Instruction::PUSHS, &&L_PUSHS},
                {Instruction::RETURN, &&L_RETURN},
                {Instruction::SUB, &&L_SUB},
            };

            //  A tiny scrap of code to elegantly exit from the interpreter.
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
        _callStack.push_back( Cell{ .ref = nullptr } );     // Dummy.
        _callStack.push_back( Cell{ .refCell = &_exit_code[0] } );
        pc += ProcedureLayout::InstructionsOffset;          // Skip the procedure header.
        goto *pc++->ref;

        L_PASSIGN: {
            Ident * ident = (pc++)->refIdent;
            Cell proc{ *pc++ };
            ident->value() = proc;
            goto *(pc++->ref);
        }

        L_POP_GLOBAL: {
            Ident * ident = (pc++)->refIdent;
            ident->value() = _valueStack.back();
            _valueStack.pop_back();
            goto *(pc++->ref);
        }

        L_PUSH_GLOBAL: {
            Ident * ident = (pc++)->refIdent;
            _valueStack.push_back(ident->value());
            goto *(pc++->ref);
        }

        L_ADD: {
            Cell b = _valueStack.back();
            _valueStack.pop_back();
            Cell a =_valueStack.back();
            if (a.isSmall() && b.isSmall()) { 
                _valueStack.back() = Cell{ .i64 = ( a.i64 + b.i64 ) };
            } else {
                throw Mishap("Cannot add non-small values");
            }
            goto *(pc++->ref);
        }

        L_SUB: {
            Cell b = _valueStack.back();
            _valueStack.pop_back();
            Cell a =_valueStack.back();
            if (a.isSmall() && b.isSmall()) { 
                _valueStack.back() = Cell{ .i64 = ( a.i64 - b.i64 ) };
            } else {
                throw Mishap("Cannot subtract non-small values");
            }
            goto *(pc++->ref);
        }

        L_MUL: {
            Cell b = _valueStack.back();
            _valueStack.pop_back();
            Cell a =_valueStack.back();
            if (a.isSmall() && b.isSmall()) { 
                _valueStack.back() = Cell{ .i64 = ( a.i64 * b.i64 ) };
            } else {
                throw Mishap("Cannot multiply non-small values");
            }
            goto *(pc++->ref);
        }

        L_PUSHQ: {
            _valueStack.push_back(*pc++);
            goto *(pc++->ref);
        }

        L_PUSHS: {
            _valueStack.push_back(_valueStack.back());
            goto *(pc++->ref);
        }

        L_RETURN: {
            pc = _callStack.back().refCell;
            _callStack.pop_back();
            currentProcedure = _callStack.back().refCell;
            _callStack.pop_back();
            goto *(pc++->ref);
        }

        L_HALT: {
            if ( DEBUG ) std::cout << "DONE!" << std::endl;
            return;
        }
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

    void run(const std::string & main) {
        Ident * idptr = _symbolTable[main];
        Ident id = *idptr;
        Cell m = id.value();
        if (m.isProcedure()) {
            run(m.deref());
        } else {
            throw Mishap("Trying to call non-procedure").culprit("Entry point", main);
        }
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
    size_t _before_instructions;
    PlaceHolder _length{nullptr};

public:
    CodePlanter(Engine & engine) : 
        _engine(engine),
        _builder(engine._heap)
    {
        _builder.addCell(Cell{});
        _length = _builder.placeHolder();
        _builder.addKey(Cell{ .u64 = Cell::ProcedureTag });
        _builder.addCell(Cell::makeSmall(0));                   //  #locals
        _before_instructions = _builder.size();
    }

public:
    void addInstruction(Instruction inst) {
        Ref label_addr = _engine._opcode_map[inst];
        _builder.addCell(Cell{ .ref = label_addr });
    }

    void addData(Cell cell) {
        _builder.addCell(cell);
    }

public:
    void addGlobal(const std::string & name) {
        if (_engine._symbolTable.find(name) == _engine._symbolTable.end()) {
            std::cerr << "Global not declared: " << name << std::endl;
        }
        _builder.addCell(Cell::makeRefIdent(_engine._symbolTable[name]));
    }


public:
    void global(const std::string & name) {
        _engine.declareGlobal(name);
    }

    void buildAndBind(const std::string & name) {
        size_t after_instructions = _builder.size();
        _length.setCell( Cell::makeSmall( after_instructions - _before_instructions ) );
        Cell * c = _builder.object();
        _engine._symbolTable[name]->value() = Cell::makePtr(c);
    }

public:
    void PUSH_GLOBAL(const std::string & name) {
        addInstruction(Instruction::PUSH_GLOBAL);
        addGlobal(name);
    }

    void POP_GLOBAL(const std::string & name) {
        addInstruction(Instruction::POP_GLOBAL);
        addGlobal(name);
    }

    void PUSHQ(int64_t i) {
        addInstruction(Instruction::PUSHQ);
        addData(Cell::makeSmall(i));
    }

    void ADD() {
        addInstruction(Instruction::ADD);
    }

    void SUB() {
        addInstruction(Instruction::SUB);
    }

    void RETURN() {
        addInstruction(Instruction::RETURN);
    }

    void HALT() {
        addInstruction(Instruction::HALT);
    }

    void PUSHS() {
        addInstruction(Instruction::PUSHS);
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
    planter.global( "x" );

    planter.PUSHQ(100);
    planter.POP_GLOBAL( "x" );
    planter.PUSH_GLOBAL( "x" );
    planter.PUSHQ(1);
    planter.SUB();
    planter.POP_GLOBAL( "x" );
    planter.RETURN();
    
    planter.global( "main" );
    planter.buildAndBind( "main" );

    engine.run( "main" );

    engine.debugDisplay();

    return EXIT_SUCCESS;
}

} // namespace poppy



int main(int argc, char **argv) {
    poppy::main(argc, argv);
}


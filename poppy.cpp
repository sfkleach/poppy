#include <cstdlib>
#include <vector>   
#include <fstream>
#include <iostream>
#include <ios>
#include <map>
#include <memory>

#include "itemizer.hpp"
#include "itemrole.hpp"
#include "cell.hpp"
#include "heap.hpp"
#include "mishap.hpp"
#include "layout.hpp"
#include "xroots.hpp"

#define DEBUG 1

namespace poppy {

enum class Instruction {
    ADD,
    CALL_GLOBAL,
    CALL_LOCAL,
    GOTO,
    HALT,
    MUL,
    PASSIGN,
    POP_GLOBAL,
    POP_LOCAL,
    PUSH_GLOBAL,
    PUSH_LOCAL,
    PUSHQ,
    PUSHS,
    RETURN,
    SUB,
};

// Required for garbage collection - using this info it is possible to scan a
// procedure looking for pointers. The nargs tells how many arguments the instruction
// has and the bitmask indicates which arguments are tagged pointers.
const char * instructionInfo( const Instruction inst, int & nargs, unsigned int & bitmask ) {
    nargs = 0;
    bitmask = 0;

    switch (inst) {
        case Instruction::PUSHQ:   
            bitmask = 0b1;
            // fallthrough!
        case Instruction::POP_GLOBAL:
        case Instruction::POP_LOCAL:
        case Instruction::PUSH_GLOBAL:
        case Instruction::PUSH_LOCAL:        
        case Instruction::CALL_GLOBAL:
        case Instruction::CALL_LOCAL:
        case Instruction::GOTO:
            nargs = 1;
            break;
        case Instruction::PASSIGN:
            nargs = 2;
            break;
        default:
            //  zero nargs & empty bitmask.
            break;
    }

    switch (inst) {
        case Instruction::ADD: return "ADD";
        case Instruction::CALL_GLOBAL: return "CALL_GLOBAL";
        case Instruction::CALL_LOCAL: return "CALL_LOCAL";
        case Instruction::GOTO: return "GOTO";
        case Instruction::HALT: return "HALT";
        case Instruction::MUL: return "MUL";
        case Instruction::PASSIGN: return "PASSIGN";
        case Instruction::POP_GLOBAL: return "POP_GLOBAL";
        case Instruction::POP_LOCAL: return "POP_LOCAL";
        case Instruction::PUSH_GLOBAL: return "PUSH_GLOBAL";
        case Instruction::PUSH_LOCAL: return "PUSH_LOCAL";
        case Instruction::PUSHQ: return "PUSHQ";
        case Instruction::PUSHS: return "PUSHS";
        case Instruction::RETURN: return "RETURN";
        case Instruction::SUB: return "SUB";
    }

    // Unreachable.
    __builtin_unreachable();
}

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

    XRootsRegistry _xrootsRegistry;

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
                {Instruction::CALL_GLOBAL, &&L_CALL_GLOBAL},
                {Instruction::CALL_LOCAL, &&L_CALL_LOCAL},
                {Instruction::GOTO, &&L_GOTO},
                {Instruction::HALT, &&L_HALT},
                {Instruction::MUL, &&L_MUL},
                {Instruction::PASSIGN, &&L_PASSIGN},
                {Instruction::POP_GLOBAL, &&L_POP_GLOBAL},
                {Instruction::POP_LOCAL, &&L_POP_LOCAL},
                {Instruction::PUSH_GLOBAL, &&L_PUSH_GLOBAL},
                {Instruction::PUSH_LOCAL, &&L_PUSH_LOCAL},
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

        Cell nextProcedure{Cell::makeSmall(0)};
        currentProcedure = pc;
        uint64_t nlocals = (currentProcedure + ProcedureLayout::NumLocalsOffset)->u64;
        _callStack.push_back( Cell{ .ref = nullptr } );     // Dummy.
        _callStack.push_back( Cell{ .refCell = &_exit_code[0] } );
        if (nlocals != 0) {
            _callStack.resize(_callStack.size() + nlocals, Cell::makeSmall(0));
        }
        pc += ProcedureLayout::InstructionsOffset;          // Skip the procedure header.
        goto *pc++->ref;

        L_GOTO: {
            int64_t delta = pc->i64;
            pc += delta;
            goto *(pc->ref);
        }

        L_PASSIGN: {
            Ident * ident = (pc++)->refIdent;
            Cell proc{ *pc++ };
            ident->value() = proc;
            goto *(pc++->ref);
        }

        L_CALL_GLOBAL: {
            Ident * ident = (pc++)->refIdent;
            nextProcedure = ident->value();
            goto COMMON_CALL;
        }

        L_CALL_LOCAL: {
            uint64_t n = pc++->u64;
            nextProcedure = *( &_callStack.back() - n );
            goto *(pc++->ref);
        }

        COMMON_CALL: {
            if (nextProcedure.isProcedure()) {
                _callStack.push_back( Cell{ .refCell = currentProcedure } );
                _callStack.push_back( Cell{ .refCell = pc } );
                currentProcedure = nextProcedure.deref();
                uint64_t nlocals = (currentProcedure + ProcedureLayout::NumLocalsOffset)->u64;
                if (nlocals != 0) {
                    _callStack.resize(_callStack.size() + nlocals, Cell::makeSmall(0));
                }
                pc = currentProcedure + ProcedureLayout::InstructionsOffset;
            } else {
                throw Mishap("Trying to call non-procedure").culprit("Value", nextProcedure.u64);
            }
            goto *(pc++->ref);
        }

        L_POP_GLOBAL: {
            Ident * ident = (pc++)->refIdent;
            ident->value() = _valueStack.back();
            _valueStack.pop_back();
            goto *(pc++->ref);
        }

        L_POP_LOCAL: {
            uint64_t n = pc++->u64;
            Cell * c = &_callStack.back() - n;
            *c = _valueStack.back();
            _valueStack.pop_back();
            goto *(pc++->ref);
        }

        L_PUSH_GLOBAL: {
            Ident * ident = (pc++)->refIdent;
            _valueStack.push_back(ident->value());
            goto *(pc++->ref);
        }

        L_PUSH_LOCAL: {
            uint64_t n = pc++->u64;
            Cell * c = &_callStack.back() - n;
            _valueStack.push_back(*c);
            goto *(pc++->ref);
        }

        L_ADD: {
            Cell b = _valueStack.back();
            _valueStack.pop_back();
            Cell a =_valueStack.back();
            if (a.isSmall() && b.isSmall()) { 
                int64_t r;
                if (__builtin_add_overflow(a.i64, b.i64, &r)) {
                    //  TODO: fix culprit for cells.
                    throw Mishap("Integer overflow trapped").culprit("Arg #1", a.i64).culprit("Arg #2", b.i64);
                } else {
                    _valueStack.back() = Cell{ .i64 = r };
                }
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
                int64_t r;
                if (__builtin_sub_overflow(a.i64, b.i64, &r)) {
                    //  TODO: fix culprit for cells.
                    throw Mishap("Integer overflow trapped").culprit("Arg #1", a.i64).culprit("Arg #2", b.i64);
                } else {
                    _valueStack.back() = Cell{ .i64 = r };
                }
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
                int64_t r;
                if (__builtin_mul_overflow( a.i64 >> 3, b.i64, &r ) ) {
                    //  TODO: fix culprit for cells.
                    throw Mishap("Integer overflow trapped").culprit("Arg #1", a.i64).culprit("Arg #2", b.i64);
                } else {
                    _valueStack.back() = Cell{ .i64 = r };
                }
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
            uint64_t nlocals = (currentProcedure + ProcedureLayout::NumLocalsOffset)->u64;
            if (nlocals != 0) {
                _callStack.resize( _callStack.size() - nlocals );
            }
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
        std::cout << "Value Stack (Bottom to Top)" << std::endl;
        for ( auto & c : _valueStack ) {
            std::cout << c.u64 << std::endl;
        }
        std::cout << std::endl;
        std::cout << "Dictionary" << std::endl;
        for (auto & [name, ident] : _symbolTable) {
            std::cout << name << " = " << ident->value().u64 << std::endl;
        }
    }
};

class Label {
    Builder & _builder;
    std::optional<size_t> _offset;
    std::vector<PlaceHolder> _placeHolders;

public:
    Label(Builder & b) : _builder(b), _offset() {}

public:

    void plantLabel() {
        size_t here = _builder._codelist.size();
        if (!_offset) {
            _builder.addCell( Cell::makeI64(static_cast<int64_t>(here) ) ); 
            _placeHolders.push_back( _builder.placeHolderJustPlanted() );
        } else {
            int64_t delta = *_offset - here;
            _builder.addCell( Cell::makeI64(delta));
        }
    }

    void setLabel() {
        size_t here = _builder._codelist.size();
        for ( auto & p : _placeHolders ) {
            int64_t there = p.getCell().i64;
            p.setCell(Cell::makeI64(here - there));
        }
        _placeHolders.clear();
        _offset = here;
    }
};

class CodePlanter {

private:
    Engine & _engine;
    Builder _builder;
    size_t _before_instructions;
    PlaceHolder _length;
    PlaceHolder _num_locals;

    //  Locals
    std::vector<std::string> locals;
    int scope_level = 0;
    size_t max_level = 0;
    std::vector<PlaceHolder> local_fixups;

    // We allocate as many extra roots as we need during code-planting and
    // dispose of them all at the end of the code-planting process.
    std::vector<XRoot> _xroots;

public:
    CodePlanter(Engine & engine) : 
        _engine(engine),
        _builder(engine._heap)
    {
        _builder.addCell(Cell{});
        _length = _builder.placeHolderJustPlanted();
        _builder.addKey(Cell{ .u64 = Cell::ProcedureTag });
        _builder.addCell(Cell::makeSmall(0));                   //  num locals
        _num_locals = _builder.placeHolderJustPlanted();
        _before_instructions = _builder.size();
    }

public:
    void debugDisplay() {
        std::map<void *, Instruction> rev_map = {};
        for (auto & [inst, addr] : _engine._opcode_map) {
            rev_map[addr] = inst;
        }

        unsigned int n = ProcedureLayout::HeaderSize;
        while ( n < _builder.size() ) {
            void * inst_addr = _builder._codelist[n].ref;
            auto it = rev_map.find(inst_addr);
            if (it != rev_map.end()) {
                int nargs;
                unsigned int bitmask;
                std::string_view name = instructionInfo(it->second, nargs, bitmask);
                std::cout << n << ") " << name << std::endl;
                n += 1;
                for (int i = 0; i < nargs; i++) {
                    std::cout << "  - " << _builder._codelist[n].u64 << std::endl;
                    n += 1;
                }
            } else {
                std::cout << n << ". BAD! " << _builder._codelist[n].u64 << std::endl;
                n += 1;
            }
        }
    }

public:
    void addInstruction(Instruction inst) {
        Ref label_addr = _engine._opcode_map[inst];
        _builder.addCell(Cell{ .ref = label_addr });
    }

    void addData(Cell cell) {
        _builder.addCell(cell);
    }

    void addRawUInt(uint64_t n) {
        _builder.addCell( Cell{ .u64 = n } );
    }

    void addGlobal(const std::string & name, Instruction inst) {
        addInstruction(inst);
        if (_engine._symbolTable.find(name) == _engine._symbolTable.end()) {
            std::cerr << "Global not declared: " << name << std::endl;
        }
        _builder.addCell(Cell::makeRefIdent(_engine._symbolTable[name]));
    }

    void addLocal(const std::string & varname, Instruction inst) {
        if (!tryAddLocal(varname, inst)) {
            throw Mishap("Unknown local variable").culprit("Variable", varname);
        }
    }
    
    bool tryAddLocal(const std::string & varname, Instruction inst) {
        for (int i = locals.size(); i > 0; i--) {
            if (locals[i-1] == varname) {
                addInstruction(inst);
                addRawUInt(i);  // This needs to become max_level - i.
                PlaceHolder p = _builder.placeHolderJustPlanted();
                local_fixups.push_back(p);
                return true;
            }
        }
        return false;
    }

    void addLocalOrGlobal(const std::string & name, Instruction instLocal, Instruction instGlobal) {
        if (!tryAddLocal(name, instLocal)) {
            addGlobal(name, instGlobal);
        }
    }
    

public:
    void local(const std::string & name) {
        for (int i = locals.size(); i > scope_level; i--) {
            if (locals[i-1] == name) {
                throw Mishap("Redeclaring local in the same scope").culprit("Local", name);
            }
        }
        locals.push_back(name);
        if (locals.size() > max_level) {
            max_level = locals.size();
        }
    }

    void global(const std::string & name) {
        _engine.declareGlobal(name);
    }

    Cell * build() {
        Cell * p = _builder.object();

        //  Protect from garbage collection for the duration of this code planter.
        _xroots.emplace_back(&_engine._xrootsRegistry, Cell::makePtr(p));

        return p;
    }

    void buildAndBind(const std::string & name) {
        size_t after_instructions = _builder.size();
        _length.setCell( Cell::makeSmall( after_instructions - _before_instructions ) );
        _num_locals.setCell( Cell::makeU64(max_level) );

        // Fix up the local variable offsets.
        for (auto & p : local_fixups) {
            uint64_t n = p.getCell().u64;
            uint64_t new_n = max_level - n;
            p.setCell( Cell{ .u64 = new_n } );
        }

        Cell * c = _builder.object();
        _engine._symbolTable[name]->value() = Cell::makePtr(c);
    }

    Label newLabel() {
        return Label(_builder);
    }

public:
    void LABEL( Label & label ) {
        label.setLabel();
    }

    void GOTO( Label & label ) {
        addInstruction(Instruction::GOTO);
        label.plantLabel();
    }

    void CALL_GLOBAL(const std::string & name) {
        addGlobal(name, Instruction::CALL_GLOBAL);
    }

    void CALL_LOCAL(const std::string & name) {
        addLocal(name, Instruction::CALL_LOCAL);
    }

    void CALL(const std::string & name) {
        addLocalOrGlobal(name, Instruction::CALL_LOCAL, Instruction::CALL_GLOBAL);
    }

    void PUSH_GLOBAL(const std::string & name) {
        addGlobal(name, Instruction::PUSH_GLOBAL);
    }

    void PUSH_LOCAL(const std::string & name) {
        addLocal(name, Instruction::PUSH_LOCAL);
    }

    void PUSH(const std::string & name) {
        addLocalOrGlobal(name, Instruction::PUSH_LOCAL, Instruction::PUSH_GLOBAL);
    }

    void POP_GLOBAL(const std::string & name) {
        addGlobal(name, Instruction::POP_GLOBAL);
    }

    void POP_LOCAL(const std::string & name) {
        addLocal(name, Instruction::POP_LOCAL);
    }

    void POP( const std::string & name ) {
        addLocalOrGlobal(name, Instruction::POP_LOCAL, Instruction::POP_GLOBAL);
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

    void MUL() {
        addInstruction(Instruction::MUL);
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

    try {

        Engine engine;
        engine.initialise();
        
        //  Test out the itemization.
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

        //  Test out the code planter.
        CodePlanter doubler(engine);
        doubler.PUSHS();
        doubler.ADD();
        doubler.RETURN();
        doubler.global( "doubler" );
        doubler.buildAndBind( "doubler" );

        CodePlanter main(engine);
        main.local( "x" );

        main.PUSHQ(50);
        main.CALL_GLOBAL("doubler");
        main.CALL_GLOBAL("doubler");
        main.POP_LOCAL( "x" );
        main.PUSH_LOCAL( "x" );

        Label target = main.newLabel();
        main.GOTO(target);

        main.PUSHQ(2);
        main.SUB();
        
        main.PUSHS();

        main.LABEL(target);
        main.RETURN();
        
        main.debugDisplay();

        main.global( "main" );
        main.buildAndBind( "main" );

        engine.run( "main" );

        engine.debugDisplay();

        return EXIT_SUCCESS;

    } catch (Mishap & mex) {
        mex.report();
        return EXIT_FAILURE;
    }
}

} // namespace poppy



int main(int argc, char **argv) {
    poppy::main(argc, argv);
}


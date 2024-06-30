#include <string>

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
#include "engine.hpp"

namespace poppy {

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
            case Instruction::IFSO:
            case Instruction::IFNOT:
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
            case Instruction::IFNOT: return "IFNOT";
            case Instruction::IFSO: return "IFSO";
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

    void Engine::declareGlobal(const std::string & name) {
        if (_symbolTable.find(name) != _symbolTable.end()) {
            std::cerr << "Redeclaring global: " << name << std::endl;
        }
        Ident * ident = new Ident(Cell::makeSmall(0));
        _symbolTable[name] = ident;
    }

    void Engine::init_or_run(Cell * pc, bool init) {
        // In order to get the address-of-labels into a map we need to
        // have a separate initialisation pass, so that the labels are
        // in scope while we populate the map.
        if (init) {
            _opcode_map = {
                {Instruction::ADD, &&L_ADD},
                {Instruction::CALL_GLOBAL, &&L_CALL_GLOBAL},
                {Instruction::CALL_LOCAL, &&L_CALL_LOCAL},
                {Instruction::IFNOT, &&L_IFNOT},
                {Instruction::IFSO, &&L_IFSO},
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
            #pragma GCC diagnostic push
            //#pragma GCC diagnostic ignored "-Wdangling-pointer"
            _exit_code[0] = Cell{ .ref = &&L_HALT };
            #pragma GCC diagnostic pop
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

        L_IFNOT: {
            int64_t delta = (pc++)->i64;
            Cell v = _valueStack.back();
            _valueStack.pop_back();
            if (v.isFalse()) {
                pc += delta;
                goto *(pc->ref);
            } else {
                goto *(pc++->ref);
            }
        }

        L_IFSO: {
            int64_t delta = (pc++)->i64;
            Cell v = _valueStack.back();
            _valueStack.pop_back();
            if (v.isntFalse()) {
                pc += delta;
                goto *(pc->ref);
            } else {
                goto *(pc++->ref);
            }
        }

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

    void Engine::initialise() {
        if (sizeof(Cell) != 8) {
            throw std::runtime_error("Cell is not 8 bytes");
        }
        init_or_run(nullptr, true);
    }

    void Engine::run(Cell * pc) {
        init_or_run(pc, false);
    }

    void Engine::run(const std::string & main) {
        Ident * idptr = _symbolTable[main];
        Ident id = *idptr;
        Cell m = id.value();
        if (m.isProcedure()) {
            run(m.deref());
        } else {
            throw Mishap("Trying to call non-procedure").culprit("Entry point", main);
        }
    }

    void Engine::multiLineDisplay(Cell p) {
        if (p.isSmall()) {
            std::cout << "  Value : " << p.getSmall() << std::endl;
        } else if (p.isProcedure()) {
            Cell * pk = p.deref();
            std::cout << "  Procedure" << std::endl;
            std::cout << "    NumLocals: " << (pk + ProcedureLayout::NumLocalsOffset)->u64 << std::endl;
            int pl = (pk + ProcedureLayout::LengthOffset)->getSmall();
            std::cout << "    Length   : " << pl << std::endl;
            int qb = (pk + ProcedureLayout::QBlockOffset)->getSmall();
            std::cout << "    QBlock   : " << qb << std::endl;
            for (int i = qb; i < pl; i++) {
                int offset = (pk + i)->i64;
                std::cout << "      offset[" << i - qb << "]: " << offset << " -> ";
                Cell q = pk[offset];
                std::cout << q.u64 << std::endl;
            }
        } else {
            std::cout << p.u64 << std::endl;
        }
    }

    void Engine::debugDisplay() {
        std::cout << "Value Stack (Bottom to Top)" << std::endl;
        for ( auto & c : _valueStack ) {
            std::cout << c.u64 << std::endl;
        }
        std::cout << std::endl;
        std::cout << "Dictionary" << std::endl;
        for (auto & [name, ident] : _symbolTable) {
            std::cout << name << ":" << std::endl;
            multiLineDisplay(ident->value());
        }
    }

} // namespace poppy

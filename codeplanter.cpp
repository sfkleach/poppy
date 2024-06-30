
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
#include "codeplanter.hpp"

namespace poppy {

void Label::plantLabel() {
    size_t here = _builder._codelist.size();
    if (!_offset) {
        _builder.addCell( Cell::makeI64(static_cast<int64_t>(here) ) ); 
        _placeHolders.push_back( _builder.placeHolderJustPlanted() );
    } else {
        int64_t delta = *_offset - here;
        _builder.addCell( Cell::makeI64(delta));
    }
}

void Label::setLabel() {
    size_t here = _builder._codelist.size();
    for ( auto & p : _placeHolders ) {
        int64_t there = p.getCell().i64;
        p.setCell(Cell::makeI64(here - there));
    }
    _placeHolders.clear();
    _offset = here;
}


CodePlanter::CodePlanter(Engine & engine) : 
    _engine(engine),
    _builder(engine.getHeap())
{
    _builder.addCell(Cell{});                               // qblock
    _qblock = _builder.placeHolderJustPlanted();
    _builder.addCell(Cell{});                               // length
    _length = _builder.placeHolderJustPlanted();
    _builder.addKey(ProcedureKeyValue);     // key
    _builder.addCell(Cell::makeSmall(0));                   // num locals
    _num_locals = _builder.placeHolderJustPlanted();
}

void CodePlanter::debugDisplay() {
    std::map<void *, Instruction> rev_map = {};
    for (auto & [inst, addr] : _engine._opcode_map) {
        rev_map[addr] = inst;
    }

    {
        int offset = 0;
        for (auto &cell: _builder._codelist) {
            std::cout << "[" << offset << "] " << cell.u64 << " " << std::endl;
            offset += 1;
        }
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
                std::cout << "  " << n << ") " << _builder._codelist[n].u64 << std::endl;
                n += 1;
            }
        } else {
            std::cout << n << ") BAD! " << _builder._codelist[n].u64 << std::endl;
            n += 1;
        }
    }
}

void CodePlanter::addInstruction(Instruction inst) {
    Ref label_addr = _engine._opcode_map[inst];
    _builder.addCell(Cell{ .ref = label_addr });
}

void CodePlanter::addData(Cell cell) {
    _builder.addCell(cell);
}

void CodePlanter::addDataQ(Cell cell) {
    _q_offsets.push_back(_builder.size() - ProcedureLayout::KeyOffsetFromStart);
    _builder.addCell(cell);
}

void CodePlanter::addRawUInt(uint64_t n) {
    _builder.addCell( Cell{ .u64 = n } );
}

void CodePlanter::addGlobal(const std::string & name, Instruction inst) {
    addInstruction(inst);
    auto & dict = _engine.getDictionary();
    if (dict.find(name) == dict.end()) {
        std::cerr << "Global not declared: " << name << std::endl;
    }
    _builder.addCell(Cell::makeRefIdent(dict[name]));
}

void CodePlanter::addLocal(const std::string & varname, Instruction inst) {
    if (!tryAddLocal(varname, inst)) {
        throw Mishap("Unknown local variable").culprit("Variable", varname);
    }
}

bool CodePlanter::tryAddLocal(const std::string & varname, Instruction inst) {
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

void CodePlanter::addLocalOrGlobal(const std::string & name, Instruction instLocal, Instruction instGlobal) {
    if (!tryAddLocal(name, instLocal)) {
        addGlobal(name, instGlobal);
    }
}


void CodePlanter::local(const std::string & name) {
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

void CodePlanter::global(const std::string & name) {
    _engine.declareGlobal(name);
}

Cell * CodePlanter::build() {
    // Add the Q-block
    _qblock.setCell(Cell::makeSmall(_builder.size() - ProcedureLayout::KeyOffsetFromStart));
    for (auto &q : _q_offsets) {
        this->addRawUInt(q);
    }

    _length.setCell( Cell::makeSmall( _builder.size() - ProcedureLayout::KeyOffsetFromStart) );
    _num_locals.setCell( Cell::makeU64(max_level) );

    // Fix up the local variable offsets.
    for (auto & p : local_fixups) {
        uint64_t n = p.getCell().u64;
        uint64_t new_n = max_level - n;
        p.setCell( Cell{ .u64 = new_n } );
    }
    Cell * p = _builder.object();

    //  Protect from garbage collection for the duration of this code planter.
    _xroots.emplace_back(&_engine._xrootsRegistry, Cell::makePtr(p));

    return p;
}

void CodePlanter::buildAndBind(const std::string & name) {
    // Add the Q-block
    std::cout << "Q-block offset: " << _builder.size()  - ProcedureLayout::KeyOffsetFromStart << std::endl;
    std::cout << "Q-block size:   " << _q_offsets.size() << std::endl;
    _qblock.setCell(Cell::makeSmall(_builder.size() - ProcedureLayout::KeyOffsetFromStart));
    for (auto &q : _q_offsets) {
        this->addRawUInt(q);
    }

    _length.setCell( Cell::makeSmall( _builder.size() - ProcedureLayout::KeyOffsetFromStart ) );
    _num_locals.setCell( Cell::makeU64(max_level) );

    // Fix up the local variable offsets.
    for (auto & p : local_fixups) {
        uint64_t n = p.getCell().u64;
        uint64_t new_n = max_level - n;
        p.setCell( Cell{ .u64 = new_n } );
    }

    Cell * c = _builder.object();
    _engine.getDictionary()[name]->value() = Cell::makePtr(c);
}

Label CodePlanter::newLabel() {
    return Label(_builder);
}

void CodePlanter::LABEL( Label & label ) {
    label.setLabel();
}

void CodePlanter::GOTO( Label & label ) {
    addInstruction(Instruction::GOTO);
    label.plantLabel();
}

void CodePlanter::IFNOT( Label & label ) {
    addInstruction(Instruction::IFNOT);
    label.plantLabel();
}

void CodePlanter::IFSO( Label & label ) {
    addInstruction(Instruction::IFSO);
    label.plantLabel();
}

void CodePlanter::CALL_GLOBAL(const std::string & name) {
    addGlobal(name, Instruction::CALL_GLOBAL);
}

void CodePlanter::CALL_LOCAL(const std::string & name) {
    addLocal(name, Instruction::CALL_LOCAL);
}

void CodePlanter::CALL(const std::string & name) {
    addLocalOrGlobal(name, Instruction::CALL_LOCAL, Instruction::CALL_GLOBAL);
}

void CodePlanter::PUSH_GLOBAL(const std::string & name) {
    addGlobal(name, Instruction::PUSH_GLOBAL);
}

void CodePlanter::PUSH_LOCAL(const std::string & name) {
    addLocal(name, Instruction::PUSH_LOCAL);
}

void CodePlanter::PUSH(const std::string & name) {
    addLocalOrGlobal(name, Instruction::PUSH_LOCAL, Instruction::PUSH_GLOBAL);
}

void CodePlanter::POP_GLOBAL(const std::string & name) {
    addGlobal(name, Instruction::POP_GLOBAL);
}

void CodePlanter::POP_LOCAL(const std::string & name) {
    addLocal(name, Instruction::POP_LOCAL);
}

void CodePlanter::POP( const std::string & name ) {
    addLocalOrGlobal(name, Instruction::POP_LOCAL, Instruction::POP_GLOBAL);
}

void CodePlanter::PUSHQ(int64_t i) {
    addInstruction(Instruction::PUSHQ);
    addDataQ(Cell::makeSmall(i));
}

void CodePlanter::ADD() {
    addInstruction(Instruction::ADD);
}

void CodePlanter::SUB() {
    addInstruction(Instruction::SUB);
}

void CodePlanter::MUL() {
    addInstruction(Instruction::MUL);
}

void CodePlanter::RETURN() {
    addInstruction(Instruction::RETURN);
}

void CodePlanter::HALT() {
    addInstruction(Instruction::HALT);
}

void CodePlanter::PUSHS() {
    addInstruction(Instruction::PUSHS);
}

} // namespace poppy

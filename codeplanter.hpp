#ifndef CODEPLANTER_HPP
#define CODEPLANTER_HPP

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

class Label {
    Builder & _builder;
    std::optional<size_t> _offset;
    std::vector<PlaceHolder> _placeHolders;

public:
    Label(Builder & b) : _builder(b), _offset() {}

public:

    void plantLabel();

    void setLabel();
};

class CodePlanter {

private:
    Engine & _engine;
    Builder _builder;
    size_t _before_instructions;
    PlaceHolder _length;
    PlaceHolder _qblock;
    PlaceHolder _num_locals;
    PlaceHolder _proc_name;

    //  Locals
    std::vector<std::string> locals;
    int scope_level = 0;
    size_t max_level = 0;
    std::vector<PlaceHolder> local_fixups;

    // Pointer offsets
    std::vector<int>  _q_offsets;

    // We allocate as many extra roots as we need during code-planting and
    // dispose of them all at the end of the code-planting process.
    std::vector<XRoot> _xroots;

public:
    CodePlanter(Engine & engine);

public:
    void debugDisplay();

public:
    void addInstruction(Instruction inst);

    void addData(Cell cell);
    void addDataQ(Cell cell);

    void addRawUInt(uint64_t n);

    void addGlobal(const std::string & name, Instruction inst);

    void addLocal(const std::string & varname, Instruction inst);
    
    bool tryAddLocal(const std::string & varname, Instruction inst);

    void addLocalOrGlobal(const std::string & name, Instruction instLocal, Instruction instGlobal);

public:
    void local(const std::string & name);

    void global(const std::string & name);

    Cell * build();

    void buildAndBind(const std::string & name);

    Label newLabel();

public:
    void LABEL( Label & label );

    void GOTO( Label & label );

    void IFNOT( Label & label );

    void IFSO( Label & label );

    void CALL_GLOBAL(const std::string & name);

    void CALL_LOCAL(const std::string & name);

    void CALL(const std::string & name);

    void PUSH_GLOBAL(const std::string & name);

    void PUSH_LOCAL(const std::string & name);

    void PUSH(const std::string & name);

    void POP_GLOBAL(const std::string & name);

    void POP_LOCAL(const std::string & name);

    void POP( const std::string & name );

    void PUSHQ(int64_t i);

    void ADD();

    void SUB();

    void MUL();

    void RETURN();

    void HALT();

    void PUSHS();
};

#endif // CODEPLANTER_HPP

} // namespace poppy
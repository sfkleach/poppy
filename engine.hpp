#ifndef ENGINE_HPP
#define ENGINE_HPP

#define DEBUG 1

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

namespace poppy {

enum class Instruction {
    ADD,
    CALL_GLOBAL,
    CALL_LOCAL,
    GOTO,
    HALT,
    IFNOT,
    IFSO,
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
const char * instructionInfo( const Instruction inst, int & nargs, unsigned int & bitmask );

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
    void declareGlobal(const std::string & name);

private:
    void init_or_run(Cell * pc, bool init);

public:
    void initialise();

    void run(Cell * pc);

    void run(const std::string & main);

public:
    void debugDisplay();
    void showHeap() { _heap.showHeap(); }
private:
    void multiLineDisplay(Cell p);
};

} // namespace poppy


#endif // ENGINE_HPP
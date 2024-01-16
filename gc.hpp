#ifndef GC_HPP
#define GC_HPP

#include "cell.hpp"
#include "engine.hpp"

namespace poppy {

class GarbageCollector {
private:
    Engine & _engine;    
public:
    GarbageCollector(Engine & engine) : _engine(engine) {}
    void visitCell(Cell & cell);
    void visitObject(Cell * object);
    void enqueueObject(Cell * object);
    void scanObject(Cell * object);
};

} // namespace poppy

#endif // GC_HPP
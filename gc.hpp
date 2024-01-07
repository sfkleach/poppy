#ifndef GC_HPP
#define GC_HPP

#include "cell.hpp"
#include "engine.hpp"

namespace poppy {

class GarbageCollector {
public:
    GarbageCollector(Engine & engine);
    ~GarbageCollector();
    void visitCell(Cell & cell);
    void visitObject(Cell * object);
    void enqueueObject(Cell * object);
    void scanObject(Cell * object);
};

} // namespace poppy

#endif // GC_HPP
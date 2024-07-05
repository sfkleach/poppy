#ifndef GC_HPP
#define GC_HPP

#include "cell.hpp"
#include "engine.hpp"

namespace poppy {

class Scanner {
private:
    Heap & _from_space;
    Heap & _to_space;

public:
    Scanner(Heap & from_space, Heap & to_space) : _from_space(from_space), _to_space(to_space) {
        to_space.clear();
    }

public:
    void update(Cell & root) {
        if (root.isTaggedPtr() && root.deref()->isForwarded()) {
            Cell * new_location = forwardObject(root.deref());
            root = Cell::makePtr(new_location);
        }
    }

    void forwardObject(Cell * object) {
        Cell * start;
        Cell * end;
        object->boundaries(start, end);

        
    }
};

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
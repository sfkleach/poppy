#ifndef GC_HPP
#define GC_HPP

#include "cell.hpp"
#include "engine.hpp"
#include "iscanner.hpp"

namespace poppy {

    class Scanner: public IScanner {
    private:
        Heap & _from_space;
        Heap & _to_space;
        ptrdiff_t _offset;

    public:
        Scanner(Heap & from_space, Heap & to_space) : _from_space(from_space), _to_space(to_space) {
            to_space.clear();
            _offset = _to_space.blockStart() -_from_space.blockStart();
        }

    public:
        void update(Cell & root);

        void updateObject(CellRef object);

        Cell * forwardObject(CellRef object);

        void copyBackSpace() {
            _from_space.overwrite(_to_space);
        }
    };

    class GarbageCollector {
    private:
        Engine & _engine;   
    public:
        GarbageCollector(Engine & engine) : _engine(engine) {}

        void gc();
    };

} // namespace poppy

#endif // GC_HPP
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
        void update(Cell & root) {
            if (root.isTaggedPtr()) {
                CellRef object(root.deref());
                if (object.isForwarded()) {
                    root = Cell::makePtr(object->deref());
                } else if (object.isTaggedPtr()) {
                    Cell * new_location = forwardObject(object);
                    root = Cell::makeForwarded(new_location);
                }
            }
        }

        void updateObject(CellRef object);

        Cell * forwardObject(CellRef object) {
            Cell * start;
            Cell * end;
            object.boundaries(start, end);
            Cell * new_start = _to_space.copyRange(start, end);
            Cell * new_object = new_start + (object.cellRef - start);
            Cell * offset_adjusted = new_object - _offset;
            return offset_adjusted;
        }

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
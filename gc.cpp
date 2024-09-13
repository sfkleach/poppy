#include "cell.hpp"
#include "gc.hpp"
#include "engine.hpp"

namespace poppy {

    Cell * Scanner::forwardObject(CellRef object) {
        Cell * start;
        Cell * end;
        object.boundaries(start, end);
        Cell * new_start = _to_space.copyRange(start, end);
        Cell * new_object = new_start + (object.cellRef - start);
        Cell * offset_adjusted = new_object - _offset;
        return offset_adjusted;
    }

    void Scanner::forwardRoot(Cell & root) {
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

    void Scanner::scanObject(CellRef object) {
        switch (object.getKeyCode()) {
            case KeyCode::ProcedureKeyCode: {
                int length = object.offset(ProcedureLayout::LengthOffset)->getSmall();
                int qoffset = object.offset(ProcedureLayout::QBlockOffset)->getSmall();
                for (int d = qoffset; d < length; d += 1) {
                    CellRef keyCell = object.offset(d);
                    this->forwardRoot(*(keyCell.cellRef));
                }              
                break;
            }
            case KeyCode::VectorKeyCode: {
                int length = object.offset(VectorLayout::LengthOffset)->getSmall();
                int start = VectorLayout::ContentsOffset;
                int end = VectorLayout::ContentsOffset + length;
                for (int d = start; d < end; d += 1) {
                    CellRef keyCell = object.offset(d);
                    this->forwardRoot(*(keyCell.cellRef));
                }     
            }
            default:
                // No action needed.
                break;
        }
    }

    void GarbageCollector::gc() {
        Heap to_space;
        Scanner scanner(_engine.getHeap(), to_space);
        _engine.scanRoots(scanner);
        for (;;) {
            CellRef object = to_space.popEnqueuedObject();
            if (object.isNull())
                break;
            scanner.scanObject(object);
        }
    }


} // namespace poppy

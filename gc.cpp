#include "cell.hpp"
#include "gc.hpp"
#include "engine.hpp"

namespace poppy {

    void Scanner::updateObject(CellRef object) {
        switch (object.getKeyCode()) {
            case KeyCode::ProcedureKeyCode: {
                int length = object.offset(ProcedureLayout::LengthOffset)->getSmall();
                int qoffset = object.offset(ProcedureLayout::QBlockOffset)->getSmall();
                for (int d = qoffset; d < length; d += 1) {
                    CellRef keyCell = object.offset(d);
                    this->update(*(keyCell.cellRef));
                }              
                break;
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
            scanner.updateObject(object);
        }
    }


} // namespace poppy

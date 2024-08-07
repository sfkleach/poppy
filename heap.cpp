#include <stdexcept>
#include <iostream>

#include "heap.hpp"
#include "mishap.hpp"
#include "layout.hpp"
#include "cell.hpp"

namespace poppy {

    Heap::Heap()
    {
        size_t capacity = 1024;
        _block_start = (Cell *)aligned_alloc(sizeof(Cell), capacity);
        if (_block_start == nullptr ) {
            throw std::runtime_error("Cannot allocate heap store");
        }
        
        _block_end = _block_start + capacity;
        _working_tip = _block_start;
        _working_limit = _block_start + capacity / 2;
    }

    CellRef Heap::nextObject(CellRef keyCell) {
        switch (keyCell.keyCode()) {
            case KeyCode::ProcedureKeyCode: {
                int length = keyCell.offset(ProcedureLayout::LengthOffset)->getSmall();
                Cell * p = keyCell.cellRef + length;
                while (p < _working_tip) {
                    if (p->isKey())
                        return CellRef( p );
                    p += 1;
                }
                return CellRef();
            }
            default:
                throw Mishap("Unknown key").culprit("Key", static_cast<unsigned long>(keyCell->u64));
        }
        return CellRef();
    }

    CellRef Heap::firstObject() {
        Cell * p = _block_start;
        while (p < _working_tip) {
            if (p->isKey())
                return CellRef(p);
            p += 1;
        }
        return CellRef();
    }

    Builder::Builder(Heap & heap) : 
        _heap(heap)
    {
    }

    void Builder::addCell(Cell cell) {
        _codelist.push_back(cell);
    }

    void Builder::addKey(Cell cell) {
        _key_offset = _codelist.size();
        _codelist.push_back(cell);
    }

    size_t Builder::size() {
        return _codelist.size();
    }

    Cell * Builder::object() {
        if (_heap._working_tip + _codelist.size() >= _heap._working_limit) {
            throw std::runtime_error("Heap overflow");
        }
        Cell * result =_heap._working_tip + _key_offset;
        for (auto cell : _codelist) {
            *_heap._working_tip++ = cell;
        }
        return result;
    }

    PlaceHolder Builder::placeHolderJustPlanted() {
        if (_codelist.size() > 0 ) {
            return PlaceHolder{ &_codelist, static_cast<int>(_codelist.size()) - 1 };
        } else {
            throw Mishap("No cell planted");
        }
    }

    void Builder::debugDisplay() {
        int n = 0;
        for (auto c : _codelist) {
            std::cout << n++ << ". " << c.u64 << std::endl;
        }
        std::cout << std::endl;
    }

} // namespace poppy
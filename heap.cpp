#include <stdexcept>
#include <iostream>
#include <cstring> // for std::memcpy

#include "heap.hpp"
#include "mishap.hpp"
#include "layout.hpp"
#include "cell.hpp"

namespace poppy {

    constexpr std::size_t kapacity = 1024;

    Heap::Heap()
    {
        _block_start = (Cell *)aligned_alloc(sizeof(Cell), kapacity);
        if (_block_start == nullptr ) {
            throw std::runtime_error("Cannot allocate heap store");
        }
        
        _scan_queue = _block_start;
        _block_end = _block_start + kapacity;
        _working_tip = _block_start;
        _working_limit = _block_start + kapacity / 2;
    }

    Heap::~Heap() {
        free(_block_start);
    }

    void Heap::clear() {
        _scan_queue = _block_start;
        _working_tip = _block_start;
        _working_limit = _block_start + kapacity / 2;
    }

    CellRef Heap::popEnqueuedObject(Cell * & start, Cell * & end) {
        if (_scan_queue >= _working_tip) {
            start = nullptr;
            end = nullptr;
            return CellRef();
        } else {

            CellRef object = nextObject(CellRef(_scan_queue));
            object.boundaries(start, end);
            return object;
        }
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

    Cell * Heap::copyRange(Cell * start, Cell * end) {
        Cell * tip = _working_tip;
        ptrdiff_t delta = end - start;
        std::memcpy(_working_tip, start, delta * sizeof(Cell));
        _working_tip += delta;
        return tip;
    }

    void Heap::overwrite(const Heap & other) {
        ptrdiff_t delta = other._working_tip - other._block_start;
        _working_tip = _block_start + delta;
        std::memcpy(_block_start, other._block_start, delta * sizeof(Cell));
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
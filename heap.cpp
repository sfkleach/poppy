#include <stdexcept>

#include "heap.hpp"

namespace poppy {


Heap::Heap()
{
    _capacity = 1024;
    _data = (Cell *)calloc(_capacity, sizeof(Cell));
    _top = _data;

    if (_data == nullptr ) {
        throw std::runtime_error("Cannot allocate heap store");
    }
}

Builder::Builder(Heap & heap, Cell key) : 
    _heap(heap)
{
    _codelist.push_back(key);
    _key_offset = 0;
}

void Builder::addCell(Cell cell) {
    _codelist.push_back(cell);
}

Cell * Builder::object() {
    if (_heap._top + _codelist.size() >= _heap._data + _heap._capacity) {
        throw std::runtime_error("Heap overflow");
    }
    Cell * obj = _heap._top + _key_offset;
    for (auto cell : _codelist) {
        *_heap._top++ = cell;
    }
    return obj;
}


} // namespace poppy
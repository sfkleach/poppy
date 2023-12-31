#include <stdexcept>
#include <iostream>

#include "heap.hpp"
#include "mishap.hpp"

namespace poppy {


Heap::Heap()
{
    _capacity = 1024;
    _data = (Cell *)aligned_alloc(sizeof(Cell), _capacity);
    _top = _data;

    if (_data == nullptr ) {
        throw std::runtime_error("Cannot allocate heap store");
    }
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
    if (_heap._top + _codelist.size() >= _heap._data + _heap._capacity) {
        throw std::runtime_error("Heap overflow");
    }
    Cell * result =_heap._top + _key_offset;
    for (auto cell : _codelist) {
        *_heap._top++ = cell;
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
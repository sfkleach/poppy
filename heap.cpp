#include <cstdlib>
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
    _heap(heap),
    _object(heap._top)
{
}

Cell * Builder::object() {
    return _object;
}


} // namespace poppy
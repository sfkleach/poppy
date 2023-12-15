#ifndef HEAP_HPP
#define HEAP_HPP    

#include <vector>

#include "cell.hpp"

namespace poppy {

class Heap {
 
private:
    Cell * _data;
    Cell * _top;
    size_t _capacity;

public:
    Heap();

    friend class Builder;
};

class Builder {
private:
    Heap & _heap;
    size_t _key_offset = 0;
    std::vector<Cell> _codelist;

public:
    Builder(Heap & heap);

public:
    Cell * object();
    void addCell(Cell cell);
    void addKey(Cell cell);
    class PlaceHolder placeHolder();
    size_t size();
};

class PlaceHolder {
private:
    Cell * _cell;

public:
    PlaceHolder(Cell * cell) : _cell(cell) {}

public:
    void setCell(Cell cell);
};


}

#endif
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

public:
    std::vector<Cell> _codelist;

public:
    Builder(Heap & heap);

public:
    Cell * object();
    void addCell(Cell cell);
    void addKey(Cell cell);
    class PlaceHolder placeHolderJustPlanted();
    size_t size();
    void debugDisplay();
};

class PlaceHolder {
private:
    int _offset;
    std::vector<Cell> * _codelist;

public:
    PlaceHolder() : _offset(0), _codelist(nullptr) {}
    PlaceHolder(std::vector<Cell> * codelist, int offset) : 
        _offset(offset),
        _codelist(codelist)
    {}

public:
    inline void setCell(Cell cell) { (*_codelist)[_offset] = cell; }
    inline Cell getCell() { return (*_codelist)[_offset]; }
};


}

#endif
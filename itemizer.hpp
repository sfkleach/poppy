#ifndef ITEMIZER_HPP
#define Itemizer_HPP

#include <istream>

#include "item.hpp"

class Itemizer {
private:
    std::istream & _source;
public:
    Itemizer(std::istream & source) : _source(source) {};
    ~Itemizer();
    Item nextItem();
};

#endif

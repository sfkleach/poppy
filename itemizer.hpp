#ifndef ITEMIZER_HPP
#define Itemizer_HPP

#include <istream>

#include "item.hpp"

namespace poppy {

class Itemizer {
private:
    std::istream & _source;

public:
    Itemizer(std::istream & source) : _source(source) {}
    
    Item nextItem();
};

} // namespace poppy

#endif

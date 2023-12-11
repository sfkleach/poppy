#include "itemizer.hpp"

namespace poppy {

Itemizer::~Itemizer()
{
}

bool Itemizer::nextItem(Item & item) {
    item = Item("hello");
    return true;
}

} // namespace poppy
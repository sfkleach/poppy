#ifndef XROOTS_HPP
#define XROOTS_HPP

#include "cell.hpp"

namespace poppy {

class XRoot {
    friend class XRootsRegistry;
private:
    Cell _cell = Cell::makeSmall(0);
    XRoot * _next = nullptr;
    XRoot * _prev = nullptr;
public:
    XRoot(class XRootsRegistry * reg, Cell initValue);
    ~XRoot();
    inline Cell & cell() { return _cell; }
    inline XRoot * next() { return _next; }
};

class XRootsRegistry {
    XRoot _origin{ nullptr, Cell::makeSmall(0) };
public:
    void registerXRoot(class XRoot * xroot);
    XRoot * first() { return _origin._next; }
};



} // namespace poppy

#endif
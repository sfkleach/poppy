#ifndef XROOTS_HPP
#define XROOTS_HPP

#include "cell.hpp"

namespace poppy {

/*  Extra roots are a way of allocating temporary variables to hold onto 
    store, protecting them from the garbage collector for a while. This 
    protection should be short-lived. The XRootsRegistry class is used to
    keep track of all the extra roots. The XRoot class is used to represent
    each extra root.
*/

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
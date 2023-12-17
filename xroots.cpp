#include "xroots.hpp"

namespace poppy {

XRoot::XRoot(XRootsRegistry * reg, Cell initValue) : 
    _cell(initValue ) 
{
    if (reg != nullptr)
        reg->registerXRoot(this);
}

XRoot::~XRoot() {
    if (_prev) {
        _prev->_next = _next;
    }
    if (_next) {
        _next->_prev = _prev;
    }
}

void XRootsRegistry::registerXRoot(XRoot * xroot) {
    xroot->_prev = &_origin;
    xroot->_next = _origin._next;
    _origin._next = xroot;
}

} // namespace poppy
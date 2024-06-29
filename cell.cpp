#include <iostream>

#include "cell.hpp"

namespace poppy {

    void CellRef::showObject() {
        if (this->isProcedure()) {
            std::cout << "<procedure>";
        } else {
            std::cout << "<" << std::hex << (*this)->u64 << std::dec << ">";
        }
    }

    CellRef CellRef::offset(ptrdiff_t n) {
        return CellRef(cellRef + n);
    }

}

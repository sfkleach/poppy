#include <iostream>

#include "cell.hpp"

namespace poppy {

    void showObject(Cell * p) {
        if (isProcedure(p)) {
            std::cout << "<procedure>";
        } else {
            std::cout << "<" << std::hex << p->u64 << std::dec << ">";
        }
    }

    bool isProcedure(Cell * p) {
        return p->isProcedureKey();
    }

}

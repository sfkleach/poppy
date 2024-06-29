#include <iostream>
#include <exception>

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

    inline KeyCode Cell::keyCode() const {
        switch (getTag()) {
            case Tag::Small: return KeyCode::IntKeyCode;
            case Tag::TaggedPtr: return static_cast<KeyCode>(((u64 >> TAG_WIDTH) & 0xFFFFFFFF));
            case Tag::SmallFloat: return KeyCode::IntKeyCode;
            case Tag::Key: return KeyCode::KeyKeyCode;
            default: throw std::runtime_error("Unknown key code");
        }
    }

}

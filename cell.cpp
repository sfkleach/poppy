#include <iostream>
#include <exception>

#include "cell.hpp"

namespace poppy {

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

    void CellRef::boundaries(Cell * & start, Cell * & end) {
        switch (this->getKeyCode()) {
            case KeyCode::ProcedureKeyCode: {
                int length = this->offset(ProcedureLayout::LengthOffset)->getSmall();
                start = cellRef - ProcedureLayout::KeyOffsetFromStart;
                end = cellRef + length;
                break;
            }
            default:
                throw std::runtime_error("Unknown key code");
        }
    }

}

#include "item.hpp"

namespace poppy {

Item::Item(const char * name) : _name(name) {
    this->_itemCode = ItemCode::unknown_code;
    this->_syntaxCode = ItemRole::RESERVED;
}

ItemCode Item::itemCode() {
    return this->_itemCode;
}

ItemRole Item::itemRole() {
    return this->_syntaxCode;
}

} // namespace poppy
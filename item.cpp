#include "item.hpp"
#include "itemattrs.hpp"

namespace poppy {

Item::Item() :
    _name(""),
    _itemCode(ItemCode::unknown_code),
    _itemRole(ItemRole::UNKNOWN)
{}

poppy::Item::Item( const std::string & name ) : _name( name ) {
    ItemCode code;
    ItemRole role;
    if (lookupItemCode(name, code, role)) {
        _itemCode = code;
        _itemRole = role;
    } else {
        _itemCode = ItemCode::unknown_code;
        _itemRole = ItemRole::UNKNOWN;
    }
}

ItemCode Item::itemCode() {
    return this->_itemCode;
}

ItemRole Item::itemRole() {
    return this->_itemRole;
}

} // namespace poppy

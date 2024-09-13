#include "item.hpp"
#include "itemattrs.hpp"
#include "itemrole.hpp"

namespace poppy {

Item::Item() :
    _name(""),
    _itemCode(ItemCode::unknown_code)
{}

Item::Item( const std::string & name, ItemCode code, bool lookup ) :
    _name(name),
    _itemCode(code)
{
    if (lookup) {
        lookupCode();
    }
}

void Item::lookupCode() {
    ItemCode code;
    if (lookupItemCode(_name, code)) {
        _itemCode = code;
    }
}

ItemCode Item::itemCode() {
    return this->_itemCode;
}

ItemRole Item::itemRole() {
    return itemCodeToItemRole(this->_itemCode);
}

} // namespace poppy

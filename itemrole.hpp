#ifndef ITEMROLE_HPP
#define ITEMROLE_HPP

#include <string>

#include "itemattrs.hpp"

namespace poppy {
    enum class ItemRole { UNKNOWN, PUNCTUATION, CONSTANT, VARIABLE, PREFIX, INFIX, POSTFIX, BOTHFIX, RESERVED };
    std::string itemRoleToString(ItemRole role);
    ItemRole itemCodeToItemRole(ItemCode code);
}

#endif
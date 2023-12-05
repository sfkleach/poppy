#ifndef ITEMCODE_HPP
#define ITEMCODE_HPP

#include "keywords.hpp"
#include "itemcode.hpp"

#define X( name, code, role ) b,

enum ItemCode {
    #include "itemattrs.xpp"
};

#endif

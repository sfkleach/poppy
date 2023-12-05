#include "item.hpp"



void nameAttributes( const char *name ) {
    #include "itemattrs.xpp"
}

Item::Item(const char * name) : _name(name) {
    this->_itemCode = ItemCode::RESERVED;
    this->_syntaxCode = SyntaxCode::RESERVED;
}
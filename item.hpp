#ifndef ITEM_HPP
#define ITEM_HPP

#include <string>

#include "itemattrs.hpp"

namespace poppy {

class Item {
private:
    std::string 			_name;			//	mandatory
    
public:
    ItemCode 				_itemCode;		//	mandatory
	ItemRole 				_itemRole;		//	mandatory
    
public:
	std::string & nameString() { return this->_name; }

public:
	Item();
	Item( const std::string & name );

public:
	ItemCode itemCode();
	ItemRole itemRole();
	bool item_is_neg_num();
	bool item_is_signed_num();
	int item_int();
	bool item_is_prefix();
	bool item_is_postfix( bool break_on_nl );
	bool item_is_anyfix();
	bool item_is_binary();
	bool item_is_name();
};

} // namespace poppy

#endif

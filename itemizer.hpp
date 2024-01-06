#ifndef ITEMIZER_HPP
#define ITEMIZER_HPP

#include <istream>
#include <optional>

#include "item.hpp"

namespace poppy {    

class Itemizer {
private:
    std::optional<char> _char;
    std::istream & _source;
    std::optional<Item> _optitem;

private:
    std::optional<char> getChar();
    std::optional<char> peekChar();
    void skipChar();
    void pushbackChar(char ch);
    std::optional<char> eatWhiteSpace();
    
public:
    Itemizer(std::istream & source) : _source(source) {}
    
    ~Itemizer();

    bool nextItem(Item & item);

    std::optional<Item> nextItem();

    void pushback(const Item & item);

    std::optional<Item> tryNextItem(ItemCode code);

};

} // namespace poppy

#endif

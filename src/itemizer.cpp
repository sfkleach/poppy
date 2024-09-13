#include <optional>
#include <sstream>

#include "itemizer.hpp"
#include "item.hpp"
#include <ctype.h>

namespace poppy {

Itemizer::~Itemizer()
{
}

std::optional<char> Itemizer::getChar() {
    if (_char) {
        char ch = _char.value();
        _char.reset();
        return ch;
    } else {
        char ch;
        if (_source >> ch ) {
            return ch;
        } else {
            return std::optional<char>(); 
        }
    }
}

std::optional<char> Itemizer::peekChar() {
    if (_char) {
        return _char;
    } else {
        char ch;
        if (_source >> ch ) {
            _char = ch;
            return ch;
        } else {
            return std::optional<char>(); 
        }
    }
}

std::optional<char> Itemizer::eatWhiteSpace() {
    if (_char) {
        if (isspace(*_char)) {
            _char.reset();
        } else {
            return _char;
        }
    }
    char ch;
    while (_source >> ch) {
        if (!isspace(ch)) {
            _char = ch;
            return _char;
        } 
    }
    return std::optional<char>();
}

void Itemizer::pushbackChar(char ch) {
    _char = ch;
}

void Itemizer::skipChar() {
    if (_char) {
        _char.reset();
    } else {
        getChar();
    }
}

bool Itemizer::nextItem(Item & item) {
    std::optional<Item> optitem = nextItem();
    bool result = optitem.has_value();
    if (result) {
        item = optitem.value();
    }
    return result;
}

std::optional<Item> Itemizer::nextItem() {
    if (_optitem) {
        Item item = _optitem.value();
        _optitem.reset();
        return item;
    } else {
        std::stringstream sofar;
        std::optional<char> optch = eatWhiteSpace();
        if (optch.has_value()) {
            char ch = optch.value();
            if (ch == '_' or isalpha(ch)) {
                while ((optch = peekChar()) and (optch.value() == '_' or isalnum(optch.value()))) {
                    sofar << optch.value();
                    skipChar();
                }
                return Item(sofar.str(), ItemCode::word_code, true);
            } else if (isdigit(ch)) {
                while ((optch = peekChar()) and (optch.value() == '_' or isdigit(optch.value()))) {
                    if (optch.value() == '_') {
                        skipChar();
                    } else {
                        sofar << optch.value();
                        skipChar();
                    }
                }
                return Item(sofar.str(), ItemCode::int_code, true);
            } else {
                sofar << optch.value();
                skipChar();
                return Item(sofar.str(), ItemCode::word_code, true);
            }
        } else {
            return std::optional<Item>();
        }
    }
}

void Itemizer::pushback(const Item & item) {
    _optitem.emplace(item);
}

std::optional<Item> Itemizer::tryNextItem(ItemCode code) {
    if (!_optitem) {
        _optitem = nextItem();
    }
    if (_optitem->_itemCode == code) {
        Item item = _optitem.value();
        _optitem.reset();
        return item;
    } else {
        return std::optional<Item>();
    }
}

} // namespace poppy
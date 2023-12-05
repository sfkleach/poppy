#include <cstdlib>
#include <vector>   
#include <fstream>

#ifndef POPPY_HPP
#define POPPY_HPP

#include "itemizer.hpp"

typedef void * Ref;

class Cell {
public:
    union {
        int64_t i64;
        uint64_t u64;
        struct {
            int64_t payload : 61;
            unsigned int tag : 3;
        };
    } cell;
};

class Engine {
public:
    std::vector<Cell> valueStack;

};

int main(int argc, char **argv) {
    Engine engine;
    std::ifstream source( "poem.txt" );
    Itemizer itemizer( source );
    while (true) {
        Item item = itemizer.nextItem();
        if (item.itemType() == ItemType::RESERVED) {
            break;
        }
        std::cout << item.nameString() << std::endl;
    }
    return EXIT_SUCCESS;
}

#endif

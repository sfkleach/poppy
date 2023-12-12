#include <cstdlib>
#include <vector>   
#include <fstream>
#include <iostream>
#include <ios>

#include "itemizer.hpp"
#include "itemrole.hpp"


namespace poppy {

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
    using namespace poppy;
    Engine engine;
    
    std::ifstream source( "poem.txt" );
    source.unsetf(std::ios_base::skipws);
    Itemizer itemizer( source );
    Item item;
    while (itemizer.nextItem(item)) {
        std::cout << item.nameString() << std::endl;
        std::cout << itemRoleToString(item.itemRole()) << std::endl;
    }
    return EXIT_SUCCESS;
}

} // namespace poppy



int main(int argc, char **argv) {
    poppy::main(argc, argv);
}

#include <cstdlib>
#include <vector>   
#include <fstream>
#include <iostream>

#include "itemizer.hpp"

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
    Itemizer itemizer( source );
    Item item{""};
    while (itemizer.nextItem(item)) {
        // if (item.itemCode() == ItemCode::unknown_code) {
        //     break;
        // }
        std::cout << item.nameString() << std::endl;
    }
    return EXIT_SUCCESS;
}

} // namespace poppy



int main(int argc, char **argv) {
    poppy::main(argc, argv);
}

#include <cstdlib>
#include <vector>   
#include <fstream>
#include <iostream>
#include <ios>

#include "itemizer.hpp"
#include "itemrole.hpp"
#include "cell.hpp"


namespace poppy {



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
        const char * k = itemCodeToItemKey(item.itemCode());
        std::cout << (k == nullptr ? "?" : k) << std::endl;
        std::cout << itemRoleToString(item.itemRole()) << std::endl;
    }
    return EXIT_SUCCESS;
}

} // namespace poppy



int main(int argc, char **argv) {
    poppy::main(argc, argv);
}

#include <cstdlib>
#include <vector>   
#include <fstream>
#include <iostream>
#include <ios>
#include <map>
#include <memory>

#include "itemizer.hpp"
#include "itemrole.hpp"
#include "cell.hpp"
#include "heap.hpp"
#include "mishap.hpp"
#include "layout.hpp"
#include "xroots.hpp"
#include "engine.hpp"
#include "codeplanter.hpp"

#define DEBUG 1

namespace poppy {

int main(int argc, char **argv) {
    using namespace poppy;

    try {

        Engine engine;
        engine.initialise();
        
        //  Test out the itemization.
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

        //  Test out the code planter.
        CodePlanter doubler(engine);
        doubler.PUSHS();
        doubler.ADD();
        doubler.RETURN();
        doubler.global( "doubler" );
        doubler.buildAndBind( "doubler" );

        CodePlanter main(engine);
        main.local( "x" );

        main.PUSHQ(50);
        main.CALL_GLOBAL("doubler");
        main.CALL_GLOBAL("doubler");
        main.POP_LOCAL( "x" );
        main.PUSH_LOCAL( "x" );

        Label target = main.newLabel();
        main.GOTO(target);

        main.PUSHQ(2);
        main.SUB();
        
        main.PUSHS();

        main.LABEL(target);
        main.RETURN();
        
        
        main.global( "main" );
        main.buildAndBind( "main" );
        main.debugDisplay();

        engine.run( "main" );

        engine.debugDisplay();

        return EXIT_SUCCESS;

    } catch (Mishap & mex) {
        mex.report();
        return EXIT_FAILURE;
    }
}

} // namespace poppy



int main(int argc, char **argv) {
    poppy::main(argc, argv);
}


#ifndef CELL_HPP
#define CELL_HPP

#include <cstdlib>
#include <cstdint>

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

}

#endif
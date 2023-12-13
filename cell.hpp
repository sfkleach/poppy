#ifndef CELL_HPP
#define CELL_HPP

#include <cstdlib>
#include <cstdint>

namespace poppy {

typedef void * Ref;

enum class Tag {
    Small,          // 000 61-bit signed integer
    Ptr,            // 001 normal pointer
    PinnedPtr,      // 010 pinned pointer
    LockedPtr,      // 011 pinned and locked pointer
    EvacuatedPtr,   // 100 evacuated normal pointer
    SmallFloat,     // 101 61-bit floating point
    Special,        // 110 free for any use
    Unused          // 111 free for any use
};

enum class UpperTag {
    Key,
};

enum class KeyTag {
    KeyKey,
    ProcedureKey,
};

class Cell {
public:
    union {
        Ref ref;
        int64_t i64;
        uint64_t u64;
        struct {
            int64_t payload : 61;
            unsigned int tag : 3;
        } tagged;
        struct {
            int64_t payload : 56;
            unsigned int wideTag : 8;
        } special;
    };

public:
    inline Tag getTag() const { return (Tag)tagged.tag; }
    inline UpperTag getUpperTag() const { return (UpperTag)(special.wideTag >> 3); }
    inline unsigned char getWideTag() const { return special.wideTag; }


public:
    static const uint64_t ProcedureTag = 
        ((uint64_t)KeyTag::ProcedureKey << 8)
        | ((uint64_t)UpperTag::Key << 3) 
        | (uint64_t)Tag::Special;
    
    inline bool isProcedureKey() const { 
        return u64 == ProcedureTag;
    }
};

}

#endif
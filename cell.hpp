#ifndef CELL_HPP
#define CELL_HPP

#include <cstdlib>
#include <cstdint>

namespace poppy {

typedef void * Ref;
typedef class Ident * RefIdent;

const static unsigned int TAG_WIDTH = 3;
const static unsigned int TAG_MASK = 0x111;

enum class Tag {
    Small,          // 000 61-bit signed integer
    FreePtr,        // 001 normal pointer
    PinnedPtr,      // 010 pinned pointer
    LockedPtr,      // 011 pinned and locked pointer
    EvacuatedPtr,   // 100 evacuated normal pointer
    SmallFloat,     // 101 61-bit floating point
    Special,        // 110 free for any use
    Unused          // 111 free for any use
};

const static unsigned int UPPER_TAG_WIDTH = 5;

const static unsigned int BOTH_WIDTH = TAG_WIDTH + UPPER_TAG_WIDTH;
const static unsigned int BOTH_TAG_MASK = 0xFF;

enum class UpperTag {
    Key,                // 0000_0000 <- Reserved for system keys.
    NotDefined0,        // 0000_0001 <- Reserved for future use.
    NotDefined1,        // 0000_0010 <- Reserved for future use.
    False,              // 0001_1111 <- unique 5-bit pattern for important constant.
};

enum class KeyTag {
    KeyKey,
    ProcedureKey,
    BooleanKey,
};

class Cell {
public:
    union __attribute__((packed, aligned(8))) {
        Ref ref;
        RefIdent refIdent;
        Cell * refCell;
        int64_t i64;
        uint64_t u64;
    };

public:
    inline static Cell makeSmall(int64_t value) {
        return Cell{ .i64 = ( value << TAG_WIDTH ) };
    }

    inline static Cell makeRefIdent(RefIdent idref) {
        return Cell{ .refIdent = idref };
    }

    inline static Cell makePtr( Cell * ptr ) {
        return Cell{ .u64 = ((uint64_t)ptr | (int)Tag::FreePtr) };
    }

    inline static Cell makeU64( uint64_t n ) {
        return Cell{ .u64 = n };
    }

    inline static Cell makeI64( int64_t n ) {
        return Cell{ .i64 = n };
    }
    
public:
    inline Tag getTag() const { return (Tag)(u64 & TAG_MASK); }
    inline Tag getTag() const { return (Tag)(u64 & TAG_MASK); }
    inline UpperTag getUpperTag() const { return (UpperTag)((u64 & BOTH_TAG_MASK) >> TAG_WIDTH); }
    inline unsigned char getWideTag() const { return u64 & BOTH_TAG_MASK; }
    inline Cell * deref() const { return (Cell *)(u64 & ~TAG_WIDTH); }


public:
    static const uint64_t ProcedureTag = 
        ((uint64_t)KeyTag::ProcedureKey << BOTH_WIDTH)
        | ((uint64_t)UpperTag::Key << TAG_WIDTH) 
        | (uint64_t)Tag::Special;
    
    inline bool isProcedureKey() const { 
        return u64 == ProcedureTag;
    }

    inline bool isProcedure() const {
        return isPtr() && deref()->isProcedureKey();
    }

public:
    inline bool isSmall() const { return getTag() == Tag::Small; }
    inline bool isFalse() const { return ( u64 & 0b11111 ) == 0b11111; }
    inline bool isntFalse() const { return ( u64 & 0b11111 ) != 0b11111; }
    inline bool isPtr() const { return ((u64 & 0b100) == 0) & ((u64 & 0b011) != 0); }
};

class Ident {
private:
    class Cell _value;
public:
    Ident(Cell value) : _value(value) {}
    inline Cell & value() { return _value; }
};

}

#endif
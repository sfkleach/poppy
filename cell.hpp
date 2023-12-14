#ifndef CELL_HPP
#define CELL_HPP

#include <cstdlib>
#include <cstdint>

namespace poppy {

typedef void * Ref;
typedef class Ident * RefIdent;


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
    union __attribute__((packed, aligned(8))) {
        Ref ref;
        RefIdent refIdent;
        Cell * refCell;
        int64_t i64;
        uint64_t u64;
    };

public:
    inline static Cell makeSmall(int64_t value) {
        return Cell{ .i64 = ( value << 3 ) };
    }

    inline static Cell makeRefIdent(RefIdent idref) {
        return Cell{ .refIdent = idref };
    }
    
public:
    inline Tag getTag() const { return (Tag)(u64 & 0x7); }
    inline UpperTag getUpperTag() const { return (UpperTag)((u64 & 0xFF) >> 3); }
    inline unsigned char getWideTag() const { return u64 & 0xFF; }


public:
    static const uint64_t ProcedureTag = 
        ((uint64_t)KeyTag::ProcedureKey << 8)
        | ((uint64_t)UpperTag::Key << 3) 
        | (uint64_t)Tag::Special;
    
    inline bool isProcedureKey() const { 
        return u64 == ProcedureTag;
    }

public:
    inline bool isSmall() const { return getTag() == Tag::Small; }
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
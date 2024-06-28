#ifndef CELL_HPP
#define CELL_HPP

#include <cstdlib>
#include <cstdint>

namespace poppy {

    typedef void * Ref;
    typedef class Ident * RefIdent;

    const static uint64_t TAG_WIDTH = 3;
    const static uint64_t TAG_MASK = 0b111;

    enum class Tag {
        Small,              // 000 61-bit signed integer
        TaggedPtr,          // 001 normal pointer
        SmallFloat,         // 010 61-bit floating point
        Key,                // 011 key value
        Special,            // 100 special literal value
        EvacuatedObject,    // 101 evacuated object (points to new location)
        Unused,             // 110 free for any use
        VisitedObject       // 111 visited object (61-bits remain, reset to 011)
    };

    constexpr unsigned int UPPER_TAG_WIDTH = 5;

    constexpr unsigned int BOTH_WIDTH = TAG_WIDTH + UPPER_TAG_WIDTH;
    constexpr unsigned int BOTH_TAG_MASK = 0xFF;

    enum class UpperTag {
        False,              // 0000_0100 <- Unique false value
        True,               // 0000_1100 <- True
    };

    constexpr uint64_t FALSE_VALUE = (((int)UpperTag::False) << TAG_WIDTH) | (int)Tag::Special;
    constexpr uint64_t TRUE_VALUE = (((int)UpperTag::True) << TAG_WIDTH) | (int)Tag::Small;

    //  System keys
    enum class KeyTag {
        KeyKey,             // 0000_0011 <- Key key
        ProcedureKey,       // 0000_1011 <- Procedure key
        BooleanKey,         // 0001_0011 <- Boolean key
        IntVectorKey,       // 0001_1011 <- Int vector key
    };

    constexpr uint64_t PROCEDURE_KEY_VALUE = (((int)KeyTag::ProcedureKey) << TAG_WIDTH) | (int)Tag::Key;

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
            return Cell{ .u64 = ((uint64_t)ptr | (int)Tag::TaggedPtr) };
        }

        inline static Cell makeU64( uint64_t n ) {
            return Cell{ .u64 = n };
        }

        inline static Cell makeI64( int64_t n ) {
            return Cell{ .i64 = n };
        }
        
    public:
        inline Tag getTag() const { return (Tag)(u64 & TAG_MASK); }
        inline UpperTag getUpperTag() const { return (UpperTag)((u64 & BOTH_TAG_MASK) >> TAG_WIDTH); }
        inline unsigned char getWideTag() const { return u64 & BOTH_TAG_MASK; }
        inline Cell * deref() const { return (Cell *)(u64 & ~TAG_MASK); }


    public:
        inline bool isProcedureKey() const { 
            return u64 == PROCEDURE_KEY_VALUE;
        }

        inline bool isProcedure() const {
            return isTaggedPtr() && (deref()->u64 == PROCEDURE_KEY_VALUE);
        }

    public:
        inline bool isSmall() const { return getTag() == Tag::Small; }
        inline int getSmall() const { return i64 >> TAG_WIDTH; }

    public:
        inline bool isFalse() const { return ( u64 & 0xF ) == ( FALSE_VALUE & 0xF ); }
        inline bool isntFalse() const { return ( u64 & 0xF ) != ( FALSE_VALUE & 0xF ); }
        inline bool isTaggedPtr() const { return (u64 & TAG_MASK) == (int)Tag::TaggedPtr; }
        inline bool isKey() const { return (u64 & TAG_MASK) == (int)Tag::Key; }

    public:
        void showObject();
    };

    class Ident {
    private:
        class Cell _value;
    public:
        Ident(Cell value) : _value(value) {}
        inline Cell & value() { return _value; }
    };

    void showObject(Cell * p);
    bool isProcedure(Cell * p);

    constexpr Cell FalseValue{ .u64 = FALSE_VALUE };
    constexpr Cell TrueValue{ .u64 = TRUE_VALUE };
    constexpr Cell ProcedureKeyValue{ .u64 = PROCEDURE_KEY_VALUE };
    constexpr Cell BooleanKeyValue{ .u64 = (((int)KeyTag::BooleanKey) << TAG_WIDTH) | (int)Tag::Key };
}

#endif
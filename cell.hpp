#ifndef CELL_HPP
#define CELL_HPP

#include <cstdlib>
#include <cstdint>
#include <cstddef>

#include "layout.hpp"

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

    // The upper tag is used to implement a range of literal values that 
    // fit into 56 bits. The lower 8 bits are used to store the tag and 
    // upper tag.
    // 
    // Exceptionally, the first 2 values are used for false and true because
    // they are so common. This means that they can be tested with a simple
    // 8-bit mask. 
    enum class UpperTag {
        False,              // 0000_0100 <- Unique Talse value. 56-bit payload not used!
        True,               // 0000_1100 <- Unique True value. 56-bit payload not used!
        Sentinel,           // 0001_0100 <- Sentinels. Other singleton constants.
        Symbol              // 0001_1100 <- Symbols. 56-bit payload is an index into the symbol table.
    };

    constexpr uint64_t FALSE_VALUE = (static_cast<int>(UpperTag::False) << TAG_WIDTH) | static_cast<int>(Tag::Special);
    constexpr uint64_t TRUE_VALUE  = (static_cast<int>(UpperTag::True) << TAG_WIDTH) | static_cast<int>(Tag::Small);

    //  System keys
    enum class KeyCode {
        KeyKeyCode,             // 0000_0011 <- Key key
        ProcedureKeyCode,       // 0000_1011 <- Procedure key
        BooleanKeyCode,         // 0001_0011 <- Boolean key
        IntKeyCode,             // 0001_1011 <- Int vector key
        SymbolCode,             // 0010_1011 <- Symbol key
    };

    constexpr uint64_t PROCEDURE_KEY_VALUE = (((int)KeyCode::ProcedureKeyCode) << TAG_WIDTH) | (int)Tag::Key;


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

        inline static Cell makeForwarded( Cell * ptr ) {
            return Cell{ .u64 = ((uint64_t)ptr | (int)Tag::EvacuatedObject) };
        }

        inline static Cell makeU64( uint64_t n ) {
            return Cell{ .u64 = n };
        }

        inline static Cell makeI64( int64_t n ) {
            return Cell{ .i64 = n };
        }

        inline static Cell makeSymbol( std::size_t n ) {
            constexpr uint8_t SymbolWideTag = (static_cast<uint8_t>(UpperTag::Symbol) << TAG_WIDTH) | static_cast<uint8_t>(Tag::Special);
            return Cell{ .u64 = ( (n << BOTH_WIDTH) | SymbolWideTag ) };
        }
        
    public:
        inline Tag getTag() const { return static_cast<Tag>(u64 & TAG_MASK); }
        inline UpperTag getUpperTag() const { return static_cast<UpperTag>((u64 & BOTH_TAG_MASK) >> TAG_WIDTH); }
        inline uint8_t getWideTag() const { return u64 & BOTH_TAG_MASK; }
        inline Cell * deref() const { return (Cell *)(u64 & ~TAG_MASK); }
        inline bool isForwarded() const { return getTag() == Tag::EvacuatedObject; }
        
    public:
        inline bool IsBoolean() const {
            return (static_cast<uint8_t>(u64) & 0b11110111) == static_cast<uint8_t>(FALSE_VALUE);
        }

    public:
        inline bool isProcedureKey() const { 
            return u64 == PROCEDURE_KEY_VALUE;
        }

        inline bool isProcedure() const {
            return isTaggedPtr() && (deref()->u64 == PROCEDURE_KEY_VALUE);
        }

    public:
        inline int getSymbolIndex() const {
            return (u64 >> BOTH_WIDTH);
        }

    public:
        inline bool isSmall() const { return getTag() == Tag::Small; }
        inline int getSmall() const { return i64 >> TAG_WIDTH; }

    public:
        inline bool isFalse() const { return ( u64 & 0xF ) == ( FALSE_VALUE & 0xF ); }
        inline bool isntFalse() const { return ( u64 & 0xF ) != ( FALSE_VALUE & 0xF ); }
        inline bool isTaggedPtr() const { return (u64 & TAG_MASK) == (int)Tag::TaggedPtr; }
        inline bool isKey() const { return (u64 & TAG_MASK) == (int)Tag::Key; }
        inline KeyCode keyCode() const ;

    };

    class CellRef {
    public:
        CellRef() : cellRef(nullptr) {}
        CellRef(Cell * cell) : cellRef(cell) {}
        CellRef(const CellRef & ref) : cellRef(ref.cellRef) {}        
    public:
        Cell * cellRef;
    public:
        inline Cell * operator->() { return cellRef; }
        inline Cell & operator*() { return *cellRef; }
        CellRef offset(std::ptrdiff_t n);
    public:
        inline uint64_t u64() const { return cellRef->u64; }
    public:
        inline Tag getTag() const { return static_cast<Tag>(cellRef->u64 & TAG_MASK); }
        inline bool isForwarded() const { return getTag() == Tag::EvacuatedObject; }
        inline bool isTaggedPtr() const { return getTag() == Tag::TaggedPtr; }
        inline bool isNull() const { return cellRef == nullptr; }
        inline bool isntNull() const { return cellRef != nullptr; }
        inline bool isKey() const { return (cellRef->u64 & TAG_MASK) == static_cast<uint64_t>(Tag::Key); }
        inline KeyCode getKeyCode() const { return static_cast<KeyCode>((cellRef->u64 >> TAG_WIDTH) & 0xFFFFFFFF); }
        inline bool isProcedure() const { return cellRef->u64 == PROCEDURE_KEY_VALUE; }
        inline Cell procName() const { return cellRef[ProcedureLayout::ProcNameOffset]; }
        inline KeyCode keyCode() const { return static_cast<KeyCode>((cellRef->u64 >> TAG_WIDTH) & 0xFFFFFFFF); }
    public:
        void showObject();
        void boundaries(Cell * & start, Cell * & end);
    };


    class Ident {
    private:
        class Cell _value;
    public:
        Ident(Cell value) : _value(value) {}
        inline Cell & value() { return _value; }
    };

    constexpr Cell FalseValue{ .u64 = FALSE_VALUE };
    constexpr Cell TrueValue{ .u64 = TRUE_VALUE };
    constexpr Cell ProcedureKeyValue{ .u64 = PROCEDURE_KEY_VALUE };
    constexpr Cell BooleanKeyValue{ .u64 = (((int)KeyCode::BooleanKeyCode) << TAG_WIDTH) | (int)Tag::Key };
}

#endif
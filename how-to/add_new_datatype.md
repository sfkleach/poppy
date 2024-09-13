# How to add a new datatype: case study Vectors

## Files that are affected

- layout.hpp
- cell.\[ch\]pp
- heap.cpp
- gc.cpp

## layout.hpp

Add a set of constants that give the layout parameters for the new
datatype.

```cpp
/*  
# Vectors

1. Length:  Small
2. Key:     Key
3. Data:    Vec< Cell >
*/

class VectorLayout [
public:
    static const int LengthOffset = -1;
    static const int KeyOffsetFromStart = 1;
    static const int ContentsOffset = 1;
];
```

## cell.hpp

Add a `VectorKeyCode`, `VECTOR_KEY_VALUE` and `VectorKeyValue`.

```cpp
    enum class KeyCode [
        // ...
        VectorKeyCode,          // 00110_011 <- Vector key
    ];


    // ...
    constexpr uint64_t VECTOR_KEY_VALUE = (((int)KeyCode::VectorKeyCode) << TAG_WIDTH) | (int)Tag::Key;

    constexpr Cell VectorKeyValue{ .u64 = VECTOR_KEY_VALUE };
```

Optionally add predicates:

```cpp
        inline bool isVectorKey() const {
            return u64 == VECTOR_KEY_VALUE;
        }

        inline bool isVector() const {
            return isTaggedPtr() && (deref()->u64 == VECTOR_KEY_VALUE);
        }
```

## cell.cpp

Add another case in `boundaries`:

```cpp
    void CellRef::boundaries(Cell * & start, Cell * & end) {
        switch (this->getKeyCode()) {
            // ...
            case KeyCode::VectorKeyCode: {
                int length = this->offset(VectorLayout::LengthOffset)->getSmall();
                start = cellRef - VectorLayout::KeyOffsetFromStart;
                end = cellRef + length;
                break;
            }
            // ...
        }
    }
```

## heap.cpp

Add another case to nextObject:

```cpp
    CellRef Heap::nextObject(CellRef keyCell) {
        switch (keyCell.keyCode()) {
            // ...
            case KeyCode::VectorKeyCode: {
                int length = keyCell.offset(ProcedureLayout::LengthOffset)->getSmall();
                Cell * p = keyCell.cellRef + length;
                return findKey(p);
            }
            // ...
        }
        return CellRef();
    }
```

## gc.cpp

Add another case to `scanObject`:

```cpp
    void Scanner::scanObject(CellRef object) {
        switch (object.getKeyCode()) {
            // ...
            case KeyCode::VectorKeyCode: {
                int length = object.offset(VectorLayout::LengthOffset)->getSmall();
                int start = VectorLayout::ContentsOffset;
                int end = VectorLayout::ContentsOffset + length;
                for (int d = start; d < end; d += 1) {
                    CellRef keyCell = object.offset(d);
                    this->forwardRoot(*(keyCell.cellRef));
                }     
            }
            // ...
        }
    }
```

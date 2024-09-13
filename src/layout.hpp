#ifndef LAYOUT_HPP
#define LAYOUT_HPP

/*  
# Procedures

1. ProcName:       Symbol
2. QBlock:         Small
3. Length:         Small
4. Key:            Key
5. NumLocals:      u64
6. Instructions:   Vec< Cell >
7. QBlock:         Vec< int >          // probably should be sizedoff_t not int.
*/

class ProcedureLayout {
public:
    static const int ProcNameOffset = -3;
    static const int QBlockOffset = -2;
    static const int LengthOffset = -1;
    static const int KeyOffsetFromStart = 3;
    static const int NumLocalsOffset = 1;
    static const int InstructionsOffset = 2;
    static const int HeaderSize = KeyOffsetFromStart + InstructionsOffset;
};

/*  
# Vectors

1. Length:  Small
2. Key:     Key
3. Data:    Vec< Cell >
*/

class VectorLayout {
public:
    static const int LengthOffset = -1;
    static const int KeyOffsetFromStart = 1;
    static const int ContentsOffset = 1;
};


#endif
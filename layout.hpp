#ifndef LAYOUT_HPP
#define LAYOUT_HPP

class ProcedureLayout {
public:
    static const unsigned int HeaderSize = 4;
    static const int LengthOffset = -1;
    static const int GCPointersOffset = 1;
    static const int NumLocalsOffset = 2;
    static const int InstructionsOffset = 3;
};

#endif
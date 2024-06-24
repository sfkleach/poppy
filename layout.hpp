#ifndef LAYOUT_HPP
#define LAYOUT_HPP

class ProcedureLayout {
public:
    static const unsigned int HeaderSize = 5;
    static const int QBlockOffset = -2;
    static const int LengthOffset = -1;
    static const int NumLocalsOffset = 1;
    static const int InstructionsOffset = 2;
};

#endif
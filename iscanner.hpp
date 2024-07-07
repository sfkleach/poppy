#ifndef ISCANNER_HPP
#define ISCANNER_HPP

namespace poppy {
    class IScanner {
    public:
        virtual ~IScanner() {}
        virtual void update(Cell & root) = 0;
    };
} // namespace poppy

#endif // ISCANNER_HPP
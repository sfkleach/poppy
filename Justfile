[private]
help:
    just --list

# Run the poppy executable
run: build
    (cd src && ./poppy)

# Build the poppy executable.
build: _make_dependencies
    (cd src && make poppy)

# Creates the dependencies.makefile required for the Makefile.
_make_dependencies:
    (cd src && g++ -MM -MG *.cpp > dependencies.makefile)

# Remove all artefacts.
clean:
    rm -f *.o src/poppy src/itemattrs.hpp src/itemattrs.cpp src/dependencies.makefile src/*.gch

# Clean and then build.
rebuild: clean build

# Remove all artefacts.
clean:
    rm -f *.o poppy itemattrs.hpp itemattrs.cpp dependencies.makefile

# Build the poppy executable.
build: _make_dependencies
    make poppy

# Creates the dependencies.makefile required for the Makefile.
_make_dependencies:
    g++ -MM -MG *.cpp > dependencies.makefile

# Clean and then build.
rebuild: clean build

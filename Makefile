################################################################################
### Standard Makefile intro
################################################################################

# Important check
MAKEFLAGS+=--warn-undefined-variables

# Causes the commands in a recipe to be issued in the same shell (beware cd commands not executed in a subshell!)
.ONESHELL:
SHELL:=/bin/bash

# When using ONESHELL, we want to exit on error (-e) and error if a command fails in a pipe (-o pipefail)
# When overriding .SHELLFLAGS one must always add a tailing `-c` as this is the default setting of Make.
.SHELLFLAGS:=-e -o pipefail -c

# Invoke the all target when no target is explicitly specified.
.DEFAULT_GOAL:=help

# Delete targets if their recipe exits with a non-zero exit code.
.DELETE_ON_ERROR:


################################################################################
### Main Contents
################################################################################

CXXFLAGS=-Wall -g -std=c++17
CPPFLAGS=
TARGET_ARCH=

poppy: poppy.o itemizer.o itemattrs.o item.o itemrole.o heap.o mishap.o xroots.o engine.o codeplanter.o
	g++ $(CXXFLAGS) -o $@ $^

include dependencies.makefile

itemattrs.cpp itemattrs.hpp: make_cpp_bchop_lookup_fn.py itemattrs.json
	python3 make_cpp_bchop_lookup_fn.py itemattrs.json


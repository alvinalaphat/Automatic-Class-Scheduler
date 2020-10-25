# initial makefile, designed for building and testing interval scheduling code

# directory alises
INC := include
SRC := src
OBJ := obj
EXE := exe

# using g++ 10.2 compiler
PP := /escnfs/home/jabbott4/public/gcc/bin/g++
LD_PATH := /escnfs/home/jabbott4/public/gcc/lib64

# Compiler flags for production quality code
CFLAGS := -O0 -g -Wall -Wextra -Wconversion -Wshadow -pedantic -Werror -I$(INC)
CXXFLAGS := -m64 -std=c++20 -Weffc++ $(CFLAGS)

# make all
all: intervalTest

# make intervalTest
intervalTestObjs := $(OBJ)/intervalTest.o $(OBJ)/Interval.o $(OBJ)/Event.o $(OBJ)/EventScheduler.o
intervalTestDeps := $(INC)/Interval.h $(INC)/Event.h $(INC)/EventScheduler.h

intervalTest: $(EXE)/intervalTest
	LD_LIBRARY_PATH=$(LD_PATH) ./$(EXE)/intervalTest

$(EXE)/intervalTest: $(intervalTestObjs)
	$(PP) $^ -o $@ $(CXXFLAGS)

$(OBJ)/%.o: $(SRC)/%.cpp $(intervalTestDeps)
	$(PP) -c -o $@ $< $(CXXFLAGS)

# make initialize
initialize:
	mkdir -p $(OBJ) $(EXE)

# make clean
clean:
	rm -rf $(OBJ)/* $(EXE)/*

.PHONY: all intervalTest

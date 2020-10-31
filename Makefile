# initial makefile, designed for building and testing interval scheduling code

# directory alises
INC := include
SRC := src
OBJ := obj
EXE := exe

# using g++ 10.2 compiler
PP := g++

# Compiler flags for production quality code
CFLAGS := -O0 -g -Wall -Wextra -Wconversion -Wshadow -pedantic -Werror -I$(INC)
CXXFLAGS := -m64 -std=c++17 -Weffc++ $(CFLAGS)

# make all
all: intervalTest EventSchedulerTest

# general include dependencies
DEPS := $(INC)/Interval.h $(INC)/Event.h $(INC)/EventScheduler.h

# make intervalTest
intervalTestObjs := $(OBJ)/intervalTest.o $(OBJ)/Interval.o

intervalTest: $(EXE)/intervalTest
	./$<

$(EXE)/intervalTest: $(intervalTestObjs)
	$(PP) $^ -o $@ $(CXXFLAGS)

# make EventSchedulerTest
EventSchedulerTestObjs := $(OBJ)/EventSchedulerTest.o $(OBJ)/Interval.o $(OBJ)/Event.o $(OBJ)/EventScheduler.o

EventSchedulerTest: $(EXE)/EventSchedulerTest
	./$<

$(EXE)/EventSchedulerTest: $(EventSchedulerTestObjs)
	$(PP) $^ -o $@ $(CXXFLAGS)

# general rule for making all object files
$(OBJ)/%.o: $(SRC)/%.cpp $(DEPS)
	$(PP) -c -o $@ $< $(CXXFLAGS)

# make initialize
initialize:
	mkdir -p $(OBJ) $(EXE)

# make clean
clean:
	rm -rf $(OBJ)/* $(EXE)/*

.PHONY: all intervalTest

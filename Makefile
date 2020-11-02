# initial makefile, designed for building and testing interval scheduling code

# directory alises
INC := include
SRC := src
OBJ := obj
EXE := exe

PP := g++

CFLAGS := -O0 -g -Wall -Wextra -Wconversion -Wshadow -pedantic -Werror -I$(INC)
CXXFLAGS := -m64 -std=c++2a -Weffc++ $(CFLAGS)

# make all
all: jsonTest CatalogueTest IntervalTest EventSchedulerTest

# make JSONTest
jsonTestObjs := $(OBJ)/jsonTest.o

jsonTestDeps := $(INC)/json.hpp

jsonTest: $(EXE)/jsonTest
	$(EXE)/./jsonTest

$(EXE)/jsonTest: $(jsonTestObjs)
	$(PP) $^ -o $@ $(CXXFLAGS)

$(OBJ)/%.o: $(SRC)/%.cpp $(jsonTestDeps)
	$(PP) $< -o $@ $(CXXFLAGS) -c

# make CatalogueTest
CatalogueTestObjs := $(OBJ)/CatalogueTest.o

CatalogueTestDeps := $(INC)/Priority.hpp $(INC)/PriorityQueue.hpp

CatalogueTest: $(EXE)/CatalogueTest
	$(EXE)/./CatalogueTest

$(EXE)/CatalogueTest: $(CatalogueTestObjs)
	$(PP) $^ -o $@ $(CXXFLAGS)

$(OBJ)/%.o: $(SRC)/%.cpp $(CatalogueTestDeps)
	$(PP) $< -o $@ $(CXXFLAGS) -c

# general include dependencies
DEPS := $(INC)/Interval.h $(INC)/Event.h $(INC)/EventScheduler.h

# make IntervalTest
IntervalTestObjs := $(OBJ)/IntervalTest.o $(OBJ)/Interval.o

IntervalTest: $(EXE)/IntervalTest
	./$<

$(EXE)/IntervalTest: $(IntervalTestObjs)
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

.PHONY: all IntervalTest EventSchedulerTest initialize clean

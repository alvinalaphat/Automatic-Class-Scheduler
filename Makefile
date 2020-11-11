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
all: jsonTest CatalogueTest IntervalTest TopElemsHeapTest EventSchedulerTest FuzzyTest

# general include dependencies
DEPS := $(INC)/Interval.h $(INC)/Event.h $(INC)/EventScheduler.h $(INC)/TopElemsHeap.h $(INC)/json.h $(INC)/Catalogue.h $(INC)/Fuzzy.h

# make JSONTest
jsonTestObjs := $(OBJ)/jsonTest.o

jsonTest: $(EXE)/jsonTest
	$(EXE)/./jsonTest

$(EXE)/jsonTest: $(jsonTestObjs)
	$(PP) $^ -o $@ $(CXXFLAGS)

# make CatalogueTest
CatalogueTestObjs := $(OBJ)/CatalogueTest.o $(OBJ)/Catalogue.o $(OBJ)/Interval.o $(OBJ)/Event.o $(OBJ)/Fuzzy.o

CatalogueTest: $(EXE)/CatalogueTest
	$(EXE)/./CatalogueTest

$(EXE)/CatalogueTest: $(CatalogueTestObjs)
	$(PP) $^ -o $@ $(CXXFLAGS)

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

# make TopElemsHeapTest
TopElemsHeapTestObjs := $(OBJ)/TopElemsHeapTest.o

TopElemsHeapTest: $(EXE)/TopElemsHeapTest
	./$<

$(EXE)/TopElemsHeapTest: $(TopElemsHeapTestObjs)
	$(PP) $^ -o $@ $(CXXFLAGS)

# make FuzzyTest
FuzzyTestObjs := $(OBJ)/FuzzyTest.o $(OBJ)/Catalogue.o $(OBJ)/Interval.o $(OBJ)/Event.o $(OBJ)/Fuzzy.o

FuzzyTest: $(EXE)/FuzzyTest
	./$<

$(EXE)/FuzzyTest: $(FuzzyTestObjs)
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

.PHONY: all IntervalTest EventSchedulerTest TopElemsHeapTest FuzzyTest initialize clean

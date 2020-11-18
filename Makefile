# initial makefile, designed for building and testing interval scheduling code

# directory alises
INC := include
SRC := src
OBJ := obj
EXE := exe

PP := g++

ifeq ($(MICR_PRET_PROJ_PERF_TEST_FLAG), 1)
	PERF_TEST_FLAG := -pg
else
	PERF_TEST_FLAG := 
endif

CFLAGS := -O2 -g $(PERF_TEST_FLAG) -Wall -Wextra -Wconversion -Wshadow -pedantic -Werror -I$(INC)
CXXFLAGS := -m64 -std=c++2a -Weffc++ $(CFLAGS)

# make all
all: jsonTest CatalogueTest IntervalTest TopElemsHeapTest EventSchedulerTest SearchEngineTest Application

# general include dependencies
DEPS := $(INC)/Interval.h $(INC)/Event.h $(INC)/EventScheduler.h $(INC)/TopElemsHeap.h $(INC)/json.h $(INC)/Catalogue.h $(INC)/SharedVector.h $(INC)/Application.h

# make JSONTest
jsonTestObjs := $(OBJ)/jsonTest.o

jsonTest: $(EXE)/jsonTest
	$(EXE)/./jsonTest

$(EXE)/jsonTest: $(jsonTestObjs)
	$(PP) $^ -o $@ $(CXXFLAGS)

# make CatalogueTest
CatalogueTestObjs := $(OBJ)/CatalogueTest.o $(OBJ)/Catalogue.o $(OBJ)/Interval.o $(OBJ)/Event.o

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

# make EventSchedulerPerfTest
EventSchedulerPerfTestObjs := $(OBJ)/EventSchedulerPerfTest.o $(OBJ)/Interval.o $(OBJ)/Event.o $(OBJ)/EventScheduler.o

EventSchedulerPerfTest: $(EXE)/EventSchedulerPerfTest
	./$<

$(EXE)/EventSchedulerPerfTest: $(EventSchedulerPerfTestObjs)
	$(PP) $^ -o $@ $(CXXFLAGS)

# make TopElemsHeapTest
TopElemsHeapTestObjs := $(OBJ)/TopElemsHeapTest.o

TopElemsHeapTest: $(EXE)/TopElemsHeapTest
	./$<

$(EXE)/TopElemsHeapTest: $(TopElemsHeapTestObjs)
	$(PP) $^ -o $@ $(CXXFLAGS)

# make SearchEngineTest
SearchEngineTestObjs := $(OBJ)/SearchEngineTest.o $(OBJ)/Catalogue.o $(OBJ)/Interval.o $(OBJ)/Event.o

SearchEngineTest: $(EXE)/SearchEngineTest
	./$<

$(EXE)/SearchEngineTest: $(SearchEngineTestObjs)
	$(PP) $^ -o $@ $(CXXFLAGS)

# make Application
ApplicationObjs := $(OBJ)/Application.o $(OBJ)/EventScheduler.o $(OBJ)/Catalogue.o $(OBJ)/Interval.o $(OBJ)/Event.o

Application: $(EXE)/Application
	./$<

$(EXE)/Application: $(ApplicationObjs)
	$(PP) $^ -o $@ $(CXXFLAGS)

# make SharedVectorTest
SharedVectorTestObjs := $(OBJ)/SharedVectorTest.o

SharedVectorTest: $(EXE)/SharedVectorTest
	./$<

$(EXE)/SharedVectorTest: $(SharedVectorTestObjs)
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

.PHONY: all jsonTest IntervalTest EventSchedulerTest EventSchedulerPerfTest TopElemsHeapTest Application initialize clean

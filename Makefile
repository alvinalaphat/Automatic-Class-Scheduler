# ..
INC := include
SRC := src
OBJ := obj
EXE := exe
PP := g++
CFLAGS := -O0 -g -Wall -Wextra -Wconversion -Wshadow -pedantic -Werror -I$(INC)
CXXFLAGS := -m64 -std=c++2a -Weffc++ $(CFLAGS)
# ..
# make all
all: jsonTest
# make jsonTest
jsonTestObjs := $(OBJ)/json_test.o
jsonTestDeps := $(INC)/json.hpp
jsonTest: $(EXE)/jsonTest
	$(EXE)/./jsonTest
$(EXE)/jsonTest: $(jsonTestObjs)
	$(PP) $^ -o $@ $(CXXFLAGS)
$(OBJ)/%.o: $(SRC)/%.cpp $(jsonTestDeps)
	$(PP) $< -o $@ $(CXXFLAGS) -c
# ..
# make initialize
initialize:
	mkdir -p $(OBJ) $(EXE)
# ..
# make clean
clean:
	rm -rf $(OBJ)/* $(EXE)/*
# ..
.PHONY: all initialize clean

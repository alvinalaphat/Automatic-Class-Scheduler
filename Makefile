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
all: jsonTest PQTest PQUserTest
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
# make PQTest
PQTestObjs := $(OBJ)/PQTest.o
PQTestDeps := $(INC)/Priority.hpp $(INC)/PriorityQueue.hpp
PQTest: $(EXE)/PQTest
	$(EXE)/./PQTest
$(EXE)/PQTest: $(PQTestObjs)
	$(PP) $^ -o $@ $(CXXFLAGS)
$(OBJ)/%.o: $(SRC)/%.cpp $(PQTestDeps)
	$(PP) $< -o $@ $(CXXFLAGS) -c
# ..
# make PQUserTest
PQUserTestObjs := $(OBJ)/PQUserTest.o
PQUserTestDeps := $(INC)/Priority.hpp $(INC)/PriorityQueue.hpp
PQUserTest: $(EXE)/PQUserTest
	$(EXE)/./PQUserTest
$(EXE)/PQUserTest: $(PQUserTestObjs)
	$(PP) $^ -o $@ $(CXXFLAGS)
$(OBJ)/%.o: $(SRC)/%.cpp $(PQUserTestDeps)
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

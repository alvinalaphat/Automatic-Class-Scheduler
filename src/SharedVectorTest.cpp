// SharedVectorTest.cpp
// Test the SharedVector class - should als be tested with valgrind or other
// memory leak checker

#include "SharedVector.h"

int main() {

    // create a SharedVector with some data
    SharedVector<int> V;
    int k = 10;
    V.queue(k);
    V.flushQueue();

    // create 100 copies, and continually modify V
    for (int i = 0; i < 100; ++i) {
        SharedVector<int> B = V;
        V.queue(i);
        B.flushQueue();
        B.queue(i);
        B.flushQueue();

        // B destructor will get called here
    }

    // do a final check that all the data got added correctly
    int j = 20;
    V.queue(j);
    V.flushQueue();
    V.display();

    return 0;
}

#include "SharedVector.h"

int main() {

    SharedVector<int> V;

    int k = 10;
    V.queue(k);
    V.flushQueue();

    {
        for (int i = 0; i < 100; ++i) {
            SharedVector<int> B = V;
            V.queue(i);
            B.flushQueue();
            B.queue(i);
            B.flushQueue();
        }
    }

    int j = 20;
    V.queue(j);
    V.flushQueue();

    V.display();

    return 0;
}
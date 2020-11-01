#include <Priority.hpp>
#include <PriorityQueue.hpp>
#include <iostream>

int main()
{
    int numPriorities;

    std::cout << "How many events would you like to add? ";
    std::cin >> numPriorities;

    PriorityQueue<int> idPQ;

    // Grab all priorities from user.
    for (int i = 0; i < numPriorities; ++i) {
        int id;
        unsigned int priority;

        std::cout << "Enter id for event #" << (i + 1) << ": ";
        std::cin >> id;

        std::cout << "Enter priority for event #" << (i + 1) << ": ";
        std::cin >> priority;

        idPQ.push({ id, priority });
    }

    // Spit out all the priorities in order.
    for (int i = 0; i < numPriorities; ++i) {
        Priority<int> top = idPQ.top();
        std::cout << "Event with id " << top.value() << " goes " << (i + 1) << "th." << std::endl;
        idPQ.pop();
    }
    
    return EXIT_SUCCESS;
}
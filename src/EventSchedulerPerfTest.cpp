
#include "EventScheduler.h"
#include "stdlib.h"

int main(int argc, char ** argv) {

    int E = 100;
    int s = 5;
    unsigned int M = 1000;
    if (argc == 4) {
        E = atoi(argv[1]);
        s = atoi(argv[2]);
        M = (unsigned int)atoi(argv[3]);
    }

    /* ----------- Input that is poor for optimal scheduilng --------------- */

    EventScheduler eventSched;
    for (int i = 0; i < E; ++i) {
        std::vector<IntervalGroup> sections;
        for (int j = 0; j < s; ++j) {
            sections.push_back(
                IntervalGroup({{5 * i + j, 5 * i + j + 1}})
            );
        }

        eventSched.addEvent(Event(sections), i, i + 3);
    }

    std::cout << "done building" << std::endl;

    auto schedule = eventSched.buildApproxSchedule(M);

    // Display the optimal schedule that was found
    std::cout << std::endl << "Optimal schedule: ";
    for (auto evSec: schedule) {
        std::cout << "Event " << evSec.first << " Section " << evSec.second << ", ";
    }
    std::cout << std::endl;

    return 0;
}
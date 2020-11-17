
#include "EventScheduler.h"
#include "stdlib.h"

int main(int argc, char ** argv) {

    // set default values for E, s, and M; get them from command line arguments
    // if provided
    int E = 100; // total number of events
    int s = 5; // number of sections to schedule
    unsigned int M = 1000; // maximum number of schedules to consider
    if (argc == 4) {
        E = atoi(argv[1]);
        s = atoi(argv[2]);
        M = (unsigned int)atoi(argv[3]);
    }

    /* ----------- Input that is poor for optimal scheduilng --------------- */

    // build a set of events with no conflicts where there are E events, each 
    // with s sections
    EventScheduler eventSched(E, s);
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

    // build an optimal schedule, limiting to M schedules being considered
    auto schedule = eventSched.buildApproxSchedule(M);

    // Display the optimal schedule that was found
    std::cout << std::endl << "Optimal schedule: ";
    for (auto evSec: schedule) {
        std::cout << "Event " << evSec.first << " Section " << evSec.second << ", ";
    }
    std::cout << std::endl;

    return 0;
}

#include "EventScheduler.h"

int main() {

    /* ----------- Input that is poor for optimal scheduilng --------------- */

    EventScheduler eventSched;
    for (int i = 0; i < 100; ++i) {
        std::vector<IntervalGroup> sections;
        for (int j = 0; j < 20; ++j) {
            sections.push_back(
                IntervalGroup({{5 * i + j, 5 * i + j + 1}})
            );
        }

        eventSched.addEvent(Event(sections), i, i + 3);
    }

    std::cout << "done building" << std::endl;

    auto schedule = eventSched.buildApproxSchedule();

    // Display the optimal schedule that was found
    std::cout << std::endl << "Optimal schedule: ";
    for (auto evSec: schedule) {
        std::cout << "Event " << evSec.first << " Section " << evSec.second << ", ";
    }
    std::cout << std::endl;

    return 0;
}
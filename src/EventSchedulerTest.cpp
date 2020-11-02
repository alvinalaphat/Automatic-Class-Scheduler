
#include "EventScheduler.h"

int main() {

    // Build 3 Events with 3 sections a piece and add them to an event scheduler
    IntervalGroup e0s0({{0, 1}, {3, 4.5}, {8,9}});
    IntervalGroup e0s1({{2, 4}, {6, 8.5}});
    IntervalGroup e0s2({{3, 7}});
    Event event0({e0s0, e0s1, e0s2});

    IntervalGroup e1s0({{1, 3}, {4.5, 6}});
    IntervalGroup e1s1({{2, 2.5}, {6, 7}, {8,10}});
    IntervalGroup e1s2({{0, 2}, {5, 6}});
    Event event1({e1s0, e1s1, e1s2});

    IntervalGroup e2s0({{4, 5}, {9, 10}});
    IntervalGroup e2s1({{6, 7}, {8.5, 9.5}});
    IntervalGroup e2s2({{7, 9}});
    Event event2({e2s0, e2s1, e2s2});

    EventScheduler eventSched;
    eventSched.addEvent(event0, 803, 2.4);
    eventSched.addEvent(event1, 214, 3.8);
    eventSched.addEvent(event2, 971);

    // display the events and the detected conflicts
    eventSched.display(std::cout);

    // find an optimal schedule -- in the process this will print out the list
    // of schedules that were considered
    std::cout << std::endl;
    auto schedule = eventSched.buildOptimalSchedule();

    // Display the optimal schedule that was found
    std::cout << std::endl << "Optimal schedule: ";
    for (auto evSec: schedule) {
        std::cout << "Event " << evSec.first << " Section " << evSec.second << ", ";
    }
    std::cout << std::endl;

    return 0;
}
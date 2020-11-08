#include <iostream>
#include <Catalogue.h>
#include <EventScheduler.h>

int main()
{
  // Load catalogue of courses from json.
  Catalogue cat;
  
  if (cat.load("data/nd_courses.json") == EXIT_FAILURE) {
    std::cerr << "Failure loading course catalogue!" << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "There are " << cat.size() << " classes available for selection." << std::endl;

  // Take and parse user selection.
  EventScheduler sched;
  sched.addEvent(cat[19713400].event(), 19713400, 1.0);
  sched.addEvent(cat[19383838].event(), 19383838, 2.0);
  auto opt = sched.buildOptimalSchedule();

  for (auto event: opt) {
    std::cout << "Event " << event.first << " Section " << event.second << ", ";
  }

	return EXIT_SUCCESS;
}
#include <iostream>
#include <sstream>
#include <Catalogue.h>
#include <EventScheduler.h>

int to_int(std::string input)
{
  std::istringstream iss(input);
  int num = 0;
  iss >> num;
  return num;
}

int main()
{
  // Load catalogue of courses from json.
  Catalogue cat;
  
  if (cat.load("data/nd_courses.json") == EXIT_FAILURE) {
    std::cerr << "Failure loading course catalogue!" << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "There are " << cat.size() << " classes available for selection." << std::endl;

  // Take and parse user selection and add to scheduler.
  EventScheduler sched;
  std::string ans;

  while (
    std::cout << "Please enter a class id ('q' to finish adding): "
    && std::cin >> ans
    && ans != "q"
  ) {
    
    // Parse integer id from user, 0 if invalid.
    int id = to_int(ans);

    if (not cat.has(id)) {
      std::cout << "That class does not exist!" << std::endl;
      continue;
    }

    Entry entry = cat.get(id);

    // Print entry, and check if user actually wants to add this.
    std::cout << entry << std::endl;

    std::cout << "Do you want to add this course (y/n)? ";

    if (not (std::cin >> ans)) {
      break;
    }

    if (ans != "y") {
      continue;
    }

    // Parse integer priority from user, skip to next entry if invalid.
    std::cout << "Please enter a priority (integer value): ";

    if (not (std::cin >> ans)) {
      break;
    }

    int priority = to_int(ans);

    if (priority == 0) {
      std::cout << "Warning, priority is zero!" << std::endl;
      std::cout << "Class may not appear in schedule." << std::endl;
    }
    
    sched.addEvent(entry.event(), id, priority);

  }
  
  // Cosmetic. If user Ctrl-Ds, this makes sure terminal doesn't look strange.
  if (std::cin.eof()) {
    std::cout << std::endl;
  }

  // Build optimal schedule and print it out!
  std::cout << "------ BUILDING SCHEDULE -----" << std::endl;
  auto opt = sched.buildOptimalSchedule();

  std::cout << "---------- SCHEDULE ----------" << std::endl;
  for (auto event: opt) {
    std::cout << "Event id #" << event.first << ", Section " << event.second << std::endl;
  }
  

	return EXIT_SUCCESS;
}
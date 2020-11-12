#include <iostream>
#include <sstream>
#include <Catalogue.h>
#include <EventScheduler.h>
#include <Fuzzy.h>
#include <Comparable.h>
#include <iomanip>

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

  // Take and parse user selection and add to vector.
  EventScheduler sched;
  std::string ans;

  while (
    std::cout << "Please enter a class id ('q' to finish adding, 's' to search, 'l' to list): "
    && std::cin >> ans
    && ans != "q"
  ) {
    
    // show results if user wants to search
    if (ans == "s") {
      
      std::cout << "Please enter the approx. name of the course: ";

      std::cin.ignore(); // ignore newline after s

      if (not std::getline(std::cin, ans)) {
        break;
      }

      auto res = cat.search(ans, 10);

      std::cout << "Showing " << res.size() << " results for '" << ans << "':" << std::endl;
      std::cout << std::setw(16) << "ID" 
                << std::setw(48) << "NAME"
                << std::setw(16) << "SIMILARITY" << std::endl;

      for (const Comparable<Entry>& comp : res) {
        std::cout << std::setw(16) << comp.data().id()
                  << std::setw(48) << comp.data().name()
                  << std::setw(16) << comp.value() << std::endl;
      }

      continue;
    }

    // show current classes
    if (ans == "l") {

      sched.display(std::cout);

      continue;
    }

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
    std::cout << "Please enter a priority (integer value, higher is more preferred): ";

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
  auto opt = sched.buildOptimalSchedule();

  std::cout << "-------------------------------------------------" << std::endl;
  std::cout << std::setw(12) << "ID" << std::setw(36) << "NAME"
            << std::setw(24) << "SECTION TIMES" << std::endl;
    
  for (auto res: opt) {

    // print event in a pretty way
    Entry ent = cat.get(res.first);
    IntervalGroup section = ent.event().getSection(res.second);
    
    std::cout << std::setw(12) << ent.id()
              << std::setw(36) << ent.name()
              << std::setw(24) << section << std::endl;
  }
  

	return EXIT_SUCCESS;
}
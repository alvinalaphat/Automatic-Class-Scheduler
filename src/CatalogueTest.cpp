#include <Catalogue.h>
#include <iostream>

int main() {
  Catalogue cat;
  cat.fromJson("data/example_courses_1_OLD.json");

  std::cout << cat.size() << " events loaded." << std::endl;

  std::cout << "-----Event Ids-----" << std::endl;
  for (ID_TYPE id : cat.getIds()) {
    std::cout << id << " -> ";
    for (std::pair<double, double> time : cat.get(id)) {
      std::cout << "[" << time.first << ", " << time.second << "] ";
    }
    std::cout << std::endl;
  }

  return EXIT_SUCCESS;
}
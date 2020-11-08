#include <Catalogue.h>
#include <iostream>

int main() {
  Catalogue cat("data/example_courses_1.json");
	std::cout << "--------" << std::endl;
	std::cout << cat;
	std::cout << "--------" << std::endl;
  std::cout << "Has id 12345? " << (cat.has(12345) ? "Yes!" : "No!") << std::endl;
  std::cout << "Has id 99999? " << (cat.has(99999) ? "Yes!" : "No!") << std::endl;
  return EXIT_SUCCESS;
}
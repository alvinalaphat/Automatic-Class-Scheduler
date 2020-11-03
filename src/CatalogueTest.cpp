#include <Catalogue.h>
#include <iostream>

int main() {
  Catalogue cat("data/example_courses_1.json");
	std::cout << "--------" << std::endl;
	std::cout << cat;
	std::cout << "--------" << std::endl;
  return EXIT_SUCCESS;
}
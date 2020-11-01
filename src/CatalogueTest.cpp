#include <Catalogue.hpp>

int main() {
	Catalogue cat;
	cat.fromJson("data/example_courses_1.json");
	return EXIT_SUCCESS;
}
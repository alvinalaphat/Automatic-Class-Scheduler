
#include <iostream>
#include "Interval.h"

int main() {
	
	IntervalGroup i1({{1, 2}, {3, 4.5}, {7, 9}});
	IntervalGroup i2({{-2, 0}, {4.5, 6}});
	std::cout << "Interval1: " << i1 << std::endl;
	std::cout << "Interval2: " << i2 << std::endl;
	std::cout << "Conflict? " << (i1.intersects(i2) ? "Yes" : "No")
		<< std::endl << std::endl;

	IntervalGroup i3({{1, 2}, {3, 5}, {7, 9}});
	IntervalGroup i4({{-2, 0}, {4.5, 6}});
	std::cout << "Interval1: " << i3 << std::endl;
	std::cout << "Interval2: " << i4 << std::endl;
	std::cout << "Conflict? " << (i3.intersects(i4) ? "Yes" : "No")
		<< std::endl;

	return 0;
}

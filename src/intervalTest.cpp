
#include <iostream>
#include "Interval.h"

int main() {
	
	IntervalGroup i1({{1, 2}, {3, 5}, {7, 9}});
	IntervalGroup i2({{-2, 0}, {4.5, 6}});

	std::cout << i1.intersects(i2) << std::endl;

	return 0;
}

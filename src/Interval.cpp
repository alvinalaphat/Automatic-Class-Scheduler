
#include "Interval.h"
#include <algorithm>

// IntervalGroup constructor for an empty
IntervalGroup::IntervalGroup() : intervals() {}

// IntervalGroup constructor from a vector of times pairs
IntervalGroup::IntervalGroup(const std::vector<std::pair<double, double>>& ints) :
	intervals(ints) {
	
	std::sort(this -> intervals.begin(), this -> intervals.end());
}

// intersects
// return whether an interval group intersects with another interval group
bool IntervalGroup::intersects(const IntervalGroup& igroup) const {
	if (igroup.intervals.size() == 0 || this -> intervals.size() == 0) {
		return false;
	}

	std::size_t thisIndex = 0;
	std::size_t igroupIndex = 0;

	const std::pair<double, double> * elem1 = &this -> intervals[0];
	const std::pair<double, double> * elem2 = &igroup.intervals[0];
	
	while (true) {
		if (!(elem1 -> second <= elem2 -> first ||
			elem1 -> first >= elem2 -> second)) {
			
			return true;
		}

		if (elem1 -> first > elem2 -> first) {
			++igroupIndex;
			if (igroupIndex >= igroup.intervals.size()) {
				break;
			}
			elem2 = &igroup.intervals[igroupIndex];
		}
		else {
			++thisIndex;
			if (thisIndex >= this -> intervals.size()) {
				break;
			}
			elem1 = &this -> intervals[thisIndex];
		}
	}

	return false;
}

std::pair<double, double> IntervalGroup::getInterval(unsigned int index) const {
    return this->intervals.at(index);
}

unsigned int IntervalGroup::getIntervalSize() const {
    return (unsigned int)this->intervals.size();
}

// friend ostream output operator
std::ostream& operator<<(std::ostream& os, const IntervalGroup& igroup) {
	for (size_t i = 0; i < igroup.intervals.size(); ++i) {
		os << "[" << igroup.intervals[i].first << ", " <<
			igroup.intervals[i].second << "]";
		
		if (i != igroup.intervals.size() - 1) {
			os << ", ";
		}
	}

	return os;
}


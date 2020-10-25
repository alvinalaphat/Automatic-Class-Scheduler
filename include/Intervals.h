
#ifndef INTERVALS_H
#define INTERVALS_H

#include <vector>

class IntervalGroup {
	private:
		std::vector<std::pair<double, double>> intervals;

	public:
		// constructors
		IntervalGroup();	
		IntervalGroup(const std::vector<std::pair<double, double>>& intervals);

		bool intersects(const IntervalGroup& igroup) const;

};

#endif // INTERVALS_H


#ifndef INTERVAL_H
#define INTERVAL_H

#include <vector>
#include <iostream>

class IntervalGroup {
	private:
		std::vector<std::pair<double, double>> intervals;

	public:
		// constructors
		IntervalGroup();	
		IntervalGroup(const std::vector<std::pair<double, double>>& intervals);

		bool intersects(const IntervalGroup& igroup) const;

		friend std::ostream& operator<<(std::ostream& os,
			const IntervalGroup& igroup);

};

#endif // INTERVALS_H

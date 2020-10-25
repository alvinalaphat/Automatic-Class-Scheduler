#ifndef EVENTS_H
#define EVENTS_H

#include "Interval.h"

class Event {
	private:
		std::vector<IntervalGroup> sections;

	public:
		// constructors
		Event();
		Event(const std::vector<IntervalGroup>& secs);
		
		std::size_t size() const;
		IntervalGroup getSection(std::size_t index);
};

#endif

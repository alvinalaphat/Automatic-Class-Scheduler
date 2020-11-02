#ifndef EVENT_H
#define EVENT_H

#include "Interval.h"
#include <iostream>

class Event {
	private:
		std::vector<IntervalGroup> sections;

	public:
		// constructors
		Event();
		Event(const std::vector<IntervalGroup>& secs);
		
		std::size_t size() const;
		IntervalGroup getSection(std::size_t index) const;

		void display(std::ostream& os, const char * lineStart = "") const;
};

#endif

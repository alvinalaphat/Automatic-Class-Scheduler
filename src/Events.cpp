
#include "Events.h"

Event::Event() : sections() {}

Event::Event(const std::vector<IntervalGroup>& secs) :
	sections(secs) {}

std::size_t Event::size() const {
	return this -> sections.size();
}

IntervalGroup Event::getSection(std::size_t index) {
	return this -> sections[index];
}



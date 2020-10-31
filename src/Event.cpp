
#include "Event.h"

Event::Event() : sections() {}

Event::Event(const std::vector<IntervalGroup>& secs) :
	sections(secs) {}

std::size_t Event::size() const {
	return this -> sections.size();
}

IntervalGroup Event::getSection(std::size_t index) const {
	return this -> sections[index];
}

void Event::display(std::ostream& os, const char * lineStart) const {

	for (size_t i = 0; i < this -> size(); ++i) {
		os << lineStart << "Section " << i << ": " << this -> sections[i]
			<< std::endl;
	}
}



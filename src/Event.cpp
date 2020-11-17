
#include "Event.h"

// Event constructor for an empty event
Event::Event() : sections() {}

// Event constructor when inializing from a vector of sections
Event::Event(const std::vector<IntervalGroup>& secs) :
	sections(secs) {}

// size
// Return the number of sections an event has
std::size_t Event::size() const {
	return this -> sections.size();
}

// getSection
// get a section of an event based on the section's index
IntervalGroup Event::getSection(std::size_t index) const {
	return this -> sections[index];
}

// getSectionPtr
// get a pointer to a section of an event based on the index
const IntervalGroup * Event::getSectionPtr(std::size_t index) const {
	return &(this -> sections[index]);
}

// display
// print an event and it's sections in human readable format; optionally specify
// a string to print at the beginning of each line (intended to be used for
// indenting the output consistently)
void Event::display(std::ostream& os, const char * lineStart) const {

	for (size_t i = 0; i < this -> size(); ++i) {
		os << lineStart << "Section " << i << ": " << this -> sections[i]
			<< std::endl;
	}
}



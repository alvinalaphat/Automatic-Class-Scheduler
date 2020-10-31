
#include "EventScheduler.h" 

// EventScheduler constructor
EventScheduler::EventScheduler() : eventsToSchedule(), events(), conflicts() {}

// getSectionID
// produce the sectionID for an section given its eventID and section index
EventScheduler::SectionID EventScheduler::getSectionID(int eventID,
	unsigned int sectionIndex) const {

	return ((SectionID)eventID << 32) | (SectionID)sectionIndex;
}

// comparison operators for an EventWrapper
bool EventScheduler::EventWrapper::operator<(const EventWrapper& rhs) const {
	return this -> weight < rhs.weight;
}
bool EventScheduler::EventWrapper::operator>(const EventWrapper& rhs) const {
	return this -> weight > rhs.weight;
}

// addEvent
// add an event to the scheduler; in addition to the event itself, an integer
// id that is unique to this event scheduler must be specified; A weight
// describing the preference for this event can be included as well; if not
// specified it will default to 1.0
void EventScheduler::addEvent(const Event& event, int id, double weight) {

	// add the event to the priority queue and id lookup table, and find its
	// conflicts with other events
	this -> eventsToSchedule.push({id, event, weight});
	this -> addConflicts(id, event);
	this -> events.insert({id, event});
}

// addConflicts
// mark the conflicts between each section of an event and all other sections
// of the event
void EventScheduler::addConflicts(int id, const Event& event) {
	
	// iterate through each section of the event
	for (unsigned int secI = 0; secI < event.size(); ++secI) {
		SectionID sectionID = this -> getSectionID(id, secI);
		IntervalGroup section = event.getSection(secI);

		// this section's list of other sections that conflict with it
		std::unordered_set<SectionID> sectionConflicts;

		// check for conflicts against every other event section
		for (auto &otherEvent: this -> events) {
			for (unsigned int sec2I = 0; sec2I < otherEvent.second.size();
				++ sec2I) {
				
				// if there is a conflict, mark it in the conflicts graph going
				// both directions
				if (section.intersects(
					otherEvent.second.getSection(sec2I)
				)) {
					SectionID section2ID =
						this -> getSectionID(otherEvent.first, sec2I);
					
					this -> conflicts[section2ID].insert(sectionID);
					sectionConflicts.insert(section2ID);
				}
			}
		}

		// add the sectionID to the list of conflicts
		this -> conflicts.insert({sectionID, sectionConflicts});
	}
}

// display
// output the internal information of the Event Scheduler in human readable
// format
void EventScheduler::display(std::ostream& os) const {
	for (auto& eventInfo: this -> events) {
		int eventID = eventInfo.first;
		os << "Event id " << eventID << ":" << std::endl;
		
		const Event& event = eventInfo.second;
		for (unsigned int i = 0; i < event.size(); ++i) {
			os << "\tSection " << i << ":" << std::endl;
			os << "\t\tTimes: " << event.getSection(i) << std::endl;

			SectionID secID = this -> getSectionID(eventID, i);
			os << "\t\tConflicts: ";
			for (auto conflictID: this -> conflicts.at(secID)) {
				int conflictEventID = (int)(conflictID >> 32);
				unsigned int conflictSectionID = (unsigned int)conflictID;

				os << "Event " << conflictEventID << " Section "
					<< conflictSectionID << ", ";
			}
			os << std::endl;
		}
	}
}

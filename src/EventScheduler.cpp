
#include "EventScheduler.h" 

// EventScheduler constructor
EventScheduler::EventScheduler(unsigned int maxEvents,
	unsigned int maxSectionsPerEvent) :
	eventsToSchedule(maxEvents),
	events(),
	sections(),
	eventSectionsStartIndex(),
	conflicts(),
	maxSecPerEvent(maxSectionsPerEvent) {}

// getSectionID
// produce the sectionID for an section given its eventID and section index
EventScheduler::SectionID EventScheduler::getSectionID(unsigned int eventID,
	unsigned int sectionIndex) const {
	
	return this -> eventSectionsStartIndex.at(eventID) + sectionIndex;
}

// comparison operators for an EventWrapper so that it can be used in a priority
// queue
bool EventScheduler::ScheduleWrapper::operator<(const ScheduleWrapper& rhs) const {
	return this -> weight < rhs.weight;
}
bool EventScheduler::ScheduleWrapper::operator>(const ScheduleWrapper& rhs) const {
	return this -> weight > rhs.weight;
}
bool EventScheduler::ScheduleWrapper::operator<=(const ScheduleWrapper& rhs) const {
	return this -> weight <= rhs.weight;
}
bool EventScheduler::ScheduleWrapper::operator>=(const ScheduleWrapper& rhs) const {
	return this -> weight >= rhs.weight;
}
bool EventScheduler::ScheduleWrapper::operator==(const ScheduleWrapper& rhs) const {
	return this -> weight == rhs.weight;
}

// sectionConflictsWithSchedule
// determine whether adding a section to sched would cause a time conflict
bool EventScheduler::sectionConflictsWithSchedule(Schedule& sched,
	SectionID sec) const {

	const std::vector<bool>& sectionConflicts = this -> conflicts.at(sec);

	const SectionID * secIDs = sched.getArray();
	const size_t nSecIDs = sched.getSize();

	// check every section in the schedule against this section
	for (size_t i = 0; i < nSecIDs; ++i) {
		if (sectionConflicts[secIDs[i]]) {
			return true;
		}
	}

	return false;
}


// comparison operators for an EventWrapper so that it can be used in a priority
// queue
bool EventScheduler::EventWrapper::operator<(const EventWrapper& rhs) const {
	return this -> weight < rhs.weight;
}
bool EventScheduler::EventWrapper::operator>(const EventWrapper& rhs) const {
	return this -> weight > rhs.weight;
}
bool EventScheduler::EventWrapper::operator<=(const EventWrapper& rhs) const {
	return this -> weight <= rhs.weight;
}
bool EventScheduler::EventWrapper::operator>=(const EventWrapper& rhs) const {
	return this -> weight >= rhs.weight;
}
bool EventScheduler::EventWrapper::operator==(const EventWrapper& rhs) const {
	return this -> weight == rhs.weight;
}

// addEvent
// add an event to the scheduler; in addition to the event itself, an integer
// id that is unique to this event scheduler must be specified; A weight
// describing the preference for this event can be included as well; if not
// specified it will default to 1.0
void EventScheduler::addEvent(const Event& event, unsigned int id, double weight) {

	// add the event to the priority queue and id lookup table, and find its
	// conflicts with other events
	EventWrapper eventToSchedule = {id, event, weight};
	this -> eventsToSchedule.push(eventToSchedule);
	this -> events.insert({id, event});
	this -> eventSectionsStartIndex.insert({id, this -> sections.size()});

	// limit number of sections to meet perfomrance constraints
	unsigned int nSections = event.size() < this -> maxSecPerEvent
		? (unsigned int)event.size() 
		: this -> maxSecPerEvent;
	for (unsigned int i = 0; i < nSections; ++i) {
		this -> sections.push_back({id, i, this -> events[id].getSectionPtr(i)});
	}
}

// buildConflicts
// generate the conflicts graph all at once
void EventScheduler::buildConflicts() {

	// reset, then preallocate the conflicts vector
	size_t nSections = this -> sections.size();
	this -> conflicts.clear();
	this -> conflicts.reserve(nSections);

	// find the conflicts for each section
	for (size_t i = 0; i < nSections; ++i) {

		// preallocate the vector to hold the section's conflicts
		std::vector<bool> sectionConflicts(nSections, false);
		auto section = this -> sections[i].section;

		// reuse what was calculated for previous sections
		for (size_t j = 0; j < i; ++j) {
			sectionConflicts[j] = this -> conflicts[j][i];
		}

		// conflicts with self
		sectionConflicts[i] = true;

		// calculate conflicts with unprocessed sections
		for (size_t j = i + 1; j < nSections; ++j) {
			sectionConflicts[j] = section -> intersects(*this -> sections[j].section);
		}

		this -> conflicts.push_back(sectionConflicts);
	}
}

// display
// output the internal information of the Event Scheduler in human readable
// format
void EventScheduler::display(std::ostream& os) const {
	
	// display each event that has been added to the scheduler
	for (auto& eventInfo: this -> events) {
		int eventID = eventInfo.first;
		os << "Event id " << eventID << ":" << std::endl;
		
		// display the times and conflicts for each event section
		const Event& event = eventInfo.second;
		for (unsigned int i = 0; i < event.size(); ++i) {
			os << "\tSection " << i << ":" << std::endl;
			os << "\t\tTimes: " << event.getSection(i) << std::endl;

			// conflicts is built separately, so its information might not
			// be available
			if (this -> conflicts.size() == 0) {
				continue;
			}

			// list each conflict in format Event XXX Section XXX
			SectionID secID = this -> getSectionID(eventID, i);
			std::cout << secID << std::endl;
			os << "\t\tConflicts: ";
			for (SectionID conflictID = 0;
				conflictID < this -> conflicts.at(secID).size();
				++conflictID) {
				
				if (this -> conflicts.at(secID).at(conflictID)) {
					int conflictEventID = this -> sections[conflictID].eventID;
					unsigned int conflictSectionID =
						this -> sections[conflictID].sectionIndex;

					os << "Event " << conflictEventID << " Section "
						<< conflictSectionID << ", ";
				}
			}
			os << std::endl;
		}
	}
}


// buildOptimalSchedule
// use a simple approach that tries to add classes in priority order
// return the best schedule found in a vector where each entry contains
// first the event id and second the section index
// This is effectively a brute force attempt to find the independent set of the
// conflicts graph that has the largest combined weight
std::vector<std::pair<unsigned int, unsigned int>> EventScheduler::buildOptimalSchedule() {

	this -> buildConflicts();

	// copy the unscheduled events so that we don't overwrite the class member
	//auto unscheduled = this -> eventsToSchedule;
	std::priority_queue<EventWrapper> unscheduled;
	for (auto& unschedEvent: this -> eventsToSchedule.getElements()) {
		unscheduled.push(unschedEvent);
	}

	std::vector<ScheduleWrapper> schedules = {{0, {}}};

	while (!unscheduled.empty()) {

		// remove highest priority event
		EventWrapper ew = unscheduled.top();
		unscheduled.pop();

		// a set of schedules where we have attempted to add the new section
		std::vector<ScheduleWrapper> newSchedules;
		
		// go through each schedule and attempt to modify it
		for (auto& schedule: schedules) {

			// attempt to add each section the event to the schedule
			for (unsigned int i = 0; i < ew.event.size(); ++i) {
				
				SectionID secID = this -> getSectionID(ew.id, i);
				// check if the section can be added to the schedule without
				// creating conflicts
				if (!this -> sectionConflictsWithSchedule(schedule.sched,
					secID)) {
					
					// create a new schedule and add it to our list
					Schedule newSchedule = schedule.sched;
					newSchedule.queue(secID);
					newSchedule.flushQueue();
					double newWeight = schedule.weight + ew.weight;
					newSchedules.push_back({newWeight, newSchedule});
				}
			}
		}

		// augment the schedules already made with the new ones
		schedules.insert(schedules.end(), newSchedules.begin(), newSchedules.end());
	}

	// find the best schedule
	size_t bestIndex = 0;
	double bestWeight = 0;
	for (size_t i = 0; i < schedules.size(); ++i) {

		if (schedules[i].weight > bestWeight) {
			bestIndex = i;
			bestWeight = schedules[i].weight;
		}
	}

	// convert that schedule into the return format
	std::vector<std::pair<unsigned int, unsigned int>> retSched;

	Schedule bestSched = schedules[bestIndex].sched;
	size_t nSections = bestSched.getSize();
	const SectionID * secIDs = bestSched.getArray();

	for (size_t i = 0; i < nSections; ++i) {
		SectionID secID = secIDs[i];

		retSched.push_back({
			this -> sections.at(secID).eventID,
			this -> sections.at(secID).sectionIndex
		});
	}


	return retSched;
}

// buildApproxSchedule
// use the same technique as buildOptimalSchedule, but limit the number of
// schedules under consideration every round
std::vector<std::pair<unsigned int, unsigned int>> EventScheduler::buildApproxSchedule(
	unsigned int maxConsidered) {

	this -> buildConflicts();

	// copy the unscheduled events so that we don't overwrite the class member
	//auto unscheduled = this -> eventsToSchedule;
	std::priority_queue<EventWrapper> unscheduled;
	for (auto& unschedEvent: this -> eventsToSchedule.getElements()) {
		unscheduled.push(unschedEvent);
	}

	// initialize the schedules with an empty schedule
	TopElemsHeap<ScheduleWrapper> schedules(maxConsidered);
	ScheduleWrapper rootSchedule = {0, {}};
	schedules.push(rootSchedule);

	// a set of schedules where we have attempted to add the new section;
	// preallocate the maximum size required for a slight performance boost
	std::vector<ScheduleWrapper> newSchedules;
	newSchedules.reserve(this -> maxSecPerEvent * maxConsidered);

	while (!unscheduled.empty()) {

		// remove highest priority event
		EventWrapper ew = unscheduled.top();
		unscheduled.pop();
		
		// go through each schedule and attempt to modify it
		for (auto schedule: schedules.getElements()) {

			// attempt to add each section the event to the schedule
			for (unsigned int i = 0; i < ew.event.size(); ++i) {
				
				SectionID secID = this -> getSectionID(ew.id, i);
				// check if the section can be added to the schedule without
				// creating conflicts
				if (!this -> sectionConflictsWithSchedule(schedule.sched,
					secID)) {
					
					// create a new schedule and add it to our list
					Schedule newSchedule = schedule.sched;
					newSchedule.queue(secID);
					double newWeight = schedule.weight + ew.weight;
					newSchedules.push_back({newWeight, newSchedule});
				}
			}
		}

		// augment the schedules already made with the new ones
		for (auto& schedule: newSchedules) {
			schedules.push(schedule);
		}

		// write the latest changes to all the schedules that were kept
		for (auto& schedule: schedules.getMutElements()) {
			schedule.sched.flushQueue();
		}

		newSchedules.clear();
	}

	// find the best schedule
	size_t bestIndex = 0;
	double bestWeight = 0;
	for (size_t i = 0; i < schedules.getElements().size(); ++i) {

		if (schedules.getElements()[i].weight > bestWeight) {
			bestIndex = i;
			bestWeight = schedules.getElements()[i].weight;
		}
	}

	// convert that schedule into the return format
	std::vector<std::pair<unsigned int, unsigned int>> retSched;
	Schedule bestSched = schedules.getElements()[bestIndex].sched;
	size_t nSections = bestSched.getSize();
	const SectionID * secIDs = bestSched.getArray();

	for (size_t i = 0; i < nSections; ++i) {
		SectionID secID = secIDs[i];

		retSched.push_back({
			this -> sections.at(secID).eventID,
			this -> sections.at(secID).sectionIndex
		});
	}

	return retSched;
}
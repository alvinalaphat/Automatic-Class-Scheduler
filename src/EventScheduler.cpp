
#include "EventScheduler.h" 
#include "TopElemsHeap.h"

// EventScheduler constructor
EventScheduler::EventScheduler() : eventsToSchedule(), events(), sections(),
	eventSectionsStartIndex(), conflicts() {}

// getSectionID
// produce the sectionID for an section given its eventID and section index
EventScheduler::SectionID EventScheduler::getSectionID(int eventID,
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
	for (SectionID schedSec: sched) {
		if (sectionConflicts[schedSec]) {
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
void EventScheduler::addEvent(const Event& event, int id, double weight) {

	// add the event to the priority queue and id lookup table, and find its
	// conflicts with other events
	this -> eventsToSchedule.push({id, event, weight});
	this -> events.insert({id, event});
	this -> eventSectionsStartIndex.insert({id, this -> sections.size()});
	for (unsigned int i = 0; i < event.size(); ++i) {
		this -> sections.push_back({id, i});
	}
	this -> addConflicts(event);
}

// addConflicts
// mark the conflicts between each section of an event and all other sections
// of the event
void EventScheduler::addConflicts(const Event& event) {

	// iterate through each section of the event
	for (unsigned int secI = 0; secI < event.size(); ++secI) {
		IntervalGroup section = event.getSection(secI);

		// this section's list of other sections that conflict with it
		std::vector<bool> sectionConflicts;

		// check for conflicts against eveyr ohter section
		for (unsigned int otherSecID = 0; otherSecID < this -> conflicts.size();
			++otherSecID) {

			int eventID = this -> sections[otherSecID].first;
			unsigned int otherSecI = this -> sections[otherSecID].second;
			
			// mark whether or not there is a conflict in both locations of the
			// adjacency graph
			bool isConflict = false;
			if (section.intersects(
				this -> events.at(eventID).getSection(otherSecI)
			)) {
				isConflict = true;
			}
			this -> conflicts[otherSecID].push_back(isConflict);
			sectionConflicts.push_back(isConflict);
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

			// list each conflict in format Event XXX Section XXX
			SectionID secID = this -> getSectionID(eventID, i);
			os << "\t\tConflicts: ";
			for (SectionID conflictID = 0;
				conflictID < this -> conflicts.at(secID).size();
				++conflictID) {
				
				if (conflicts.at(secID).at(conflictID)) {
					int conflictEventID = this -> sections[conflictID].first;
					unsigned int conflictSectionID =
						this -> sections[conflictID].second;

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
std::vector<std::pair<int, unsigned int>> EventScheduler::buildOptimalSchedule()
	const {

	// copy the unscheduled events so that we don't overwrite the class member
	auto unscheduled = this -> eventsToSchedule;

	// 
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
					newSchedule.push_back(secID);
					double newWeight = schedule.weight + ew.weight;
					newSchedules.push_back({newWeight, newSchedule});
				}
			}
		}

		// augment the schedules already made with the new ones
		schedules.insert(schedules.end(), newSchedules.begin(), newSchedules.end());
	}

	// debugging -- print candidate schedules
	/*std::cout << "Potential schedules" << std::endl;
	for (auto& sched: schedules) {
		std::cout << "\tweight = " << sched.weight << ": ";

		for (SectionID secID: sched.sched) {
			std::cout << "Event " << (int)(secID >> 32) << " Section " <<
				(unsigned int)(secID) << ", ";
		}

		std::cout << std::endl;
	}*/

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
	std::vector<std::pair<int, unsigned int>> retSched;
	for (SectionID secID: schedules[bestIndex].sched) {
		
		retSched.push_back({
			this -> sections.at(secID).first,
			this -> sections.at(secID).second
		});
	}

	return retSched;
}

// buildApproxSchedule
// use the same technique as buildOptimalSchedule, but limit the number of
// schedules under consideration every round
std::vector<std::pair<int, unsigned int>> EventScheduler::buildApproxSchedule(
	unsigned int maxConsidered) const {

	// copy the unscheduled events so that we don't overwrite the class member
	auto unscheduled = this -> eventsToSchedule;

	// 
	//std::vector<ScheduleWrapper> schedules = {{0, {}}};
	TopElemsHeap<ScheduleWrapper> schedules(maxConsidered);
	schedules.push({0, {}});

	while (!unscheduled.empty()) {

		// remove highest priority event
		EventWrapper ew = unscheduled.top();
		unscheduled.pop();

		// a set of schedules where we have attempted to add the new section
		std::vector<ScheduleWrapper> newSchedules;
		
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
					newSchedule.push_back(secID);
					double newWeight = schedule.weight + ew.weight;
					newSchedules.push_back({newWeight, newSchedule});
				}
			}
		}

		// augment the schedules already made with the new ones
		for (auto& schedule: newSchedules) {
			schedules.push(schedule);
		}
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
	std::vector<std::pair<int, unsigned int>> retSched;
	for (SectionID secID: schedules.getElements()[bestIndex].sched) {
		
		retSched.push_back({
			this -> sections.at(secID).first,
			this -> sections.at(secID).second
		});
	}

	return retSched;
}
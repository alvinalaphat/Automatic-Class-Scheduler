
#ifndef EVENT_SCHEDULER_H
#define EVENT_SCHEDULER_H

#include "Interval.h"
#include "Event.h"
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <cstdint>

class EventScheduler {
	private:

		// a unique idetifier for a particular section of an event 
		typedef size_t SectionID;

		SectionID getSectionID(int eventID, unsigned int sectionIndex) const;

		// a collection of sections forms a schedule
		typedef std::vector<SectionID> Schedule;

		bool sectionConflictsWithSchedule(Schedule& sched, SectionID sec) const;

		struct ScheduleWrapper {
			double weight;
			Schedule sched;
			
			bool operator<(const ScheduleWrapper& rhs) const;
			bool operator>(const ScheduleWrapper& rhs) const;
			bool operator<=(const ScheduleWrapper& rhs) const;
			bool operator>=(const ScheduleWrapper& rhs) const;
			bool operator==(const ScheduleWrapper& rhs) const;
		};

		// A wrapper for an event object which makes it comparable by an
		// associated weight
		struct EventWrapper {
			int id;
			Event event;
			double weight;

			bool operator<(const EventWrapper& rhs) const;
			bool operator>(const EventWrapper& rhs) const;
			bool operator<=(const EventWrapper& rhs) const;
			bool operator>=(const EventWrapper& rhs) const;
			bool operator==(const EventWrapper& rhs) const;
		};

		// priority queue for maintaining the order in which events should
		// be attempted to be added to the schedule
		std::priority_queue<EventWrapper> eventsToSchedule;

		// a mapping from events ids to events
		std::unordered_map<int, Event> events;

		// a list of sections; you lookup a section based on its sectionID and
		// get its event id and section index
		std::vector<std::pair<int, unsigned int>> sections;

		// a mapping from event ids to their locations in the sections list
		std::unordered_map<int, size_t> eventSectionsStartIndex;

		// adjacency matrix of conflicts between sections; sections are
		std::vector<std::vector<bool>> conflicts;

		void addConflicts(const Event& event);

	public:
		EventScheduler();
		
		void addEvent(const Event& event, int id, double weight = 1.0);

		void display(std::ostream& os) const;

		std::vector<std::pair<int, unsigned int>> buildOptimalSchedule() const;
		std::vector<std::pair<int, unsigned int>> buildApproxSchedule(
			unsigned int maxConsidered = 0) const;
};

#endif // EVENT_SCHEDULER_H

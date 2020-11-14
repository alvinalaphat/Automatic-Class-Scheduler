
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

		SectionID getSectionID(unsigned int eventID, unsigned int sectionIndex) const;

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
			unsigned int id;
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
		std::unordered_map<unsigned int, Event> events;

		// a list of sections; you lookup a section based on its sectionID and
		// get its event id and section index
		struct SectionWrapper {
			unsigned int eventID;
			unsigned int sectionIndex;
			const IntervalGroup * section;
		};
		std::vector<SectionWrapper> sections;

		// a mapping from event ids to their locations in the sections list
		std::unordered_map<unsigned int, size_t> eventSectionsStartIndex;

		// adjacency matrix of conflicts between sections; sections are
		std::vector<std::vector<bool>> conflicts;

		void addConflicts(const Event& event);

		void buildConflicts();

	public:
		EventScheduler();
		
		void addEvent(const Event& event, unsigned int id, double weight = 1.0);

		void display(std::ostream& os) const;

		std::vector<std::pair<unsigned int, unsigned int>>
			buildOptimalSchedule();
		std::vector<std::pair<unsigned int, unsigned int>> buildApproxSchedule(
			unsigned int maxConsidered = 1000);
};

#endif // EVENT_SCHEDULER_H


#ifndef EVENT_SCHEDULER_H
#define EVENT_SCHEDULER_H

#include "Interval.h"
#include "Event.h"
#include "SharedVector.h"
#include "TopElemsHeap.h"
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <cstdint>

class EventScheduler {
	private:

		// a unique idetifier for a particular section of an event 
		typedef size_t SectionID;

		// a collection of sections forms a schedule
		typedef SharedVector<SectionID> Schedule;

		// A wrapper for schedules that make the comparable by associated
		// weight
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

		// a list of sections; you lookup a section based on its sectionID and
		// get its event id and section index
		struct SectionWrapper {
			unsigned int eventID;
			unsigned int sectionIndex;
			const IntervalGroup * section;
		};

		SectionID getSectionID(unsigned int eventID, unsigned int sectionIndex) const;

		bool sectionConflictsWithSchedule(Schedule& sched, SectionID sec) const;

		void buildConflicts();

		// priority queue for maintaining the order in which events should
		// be attempted to be added to the schedule
		//std::priority_queue<EventWrapper> eventsToSchedule;
		TopElemsHeap<EventWrapper> eventsToSchedule;

		// a mapping from events ids to events
		std::unordered_map<unsigned int, Event> events;

		// List of all the sections; a SectionID provides the index of the
		// vector fo looking up the section
		std::vector<SectionWrapper> sections;

		// a mapping from event ids to their locations in the sections list
		std::unordered_map<unsigned int, size_t> eventSectionsStartIndex;

		// adjacency matrix of conflicts between sections; sections are
		std::vector<std::vector<bool>> conflicts;

		// maximum number of sections considered per event
		unsigned int maxSecPerEvent;

	public:
		EventScheduler(unsigned int maxEvents=50,
			unsigned int maxSectionsPerEvent=20);
		
		void addEvent(const Event& event, unsigned int id, double weight = 1.0);

		void display(std::ostream& os) const;

		std::vector<std::pair<unsigned int, unsigned int>>
			buildOptimalSchedule();
		std::vector<std::pair<unsigned int, unsigned int>> buildApproxSchedule(
			unsigned int maxConsidered = 500);
};

#endif // EVENT_SCHEDULER_H

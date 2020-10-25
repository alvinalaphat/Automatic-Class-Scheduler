
#ifndef EVENT_SCHEDULER_H
#define EVENT_SCHEDULER_H

#include "Interval.h"
#include "Event.h"
#include <unordered_map>
#include <queue>

class EventScheduler {
	private:
		struct EventWrapper {
			Event event;
			unsigned int id;
			double weight;

			bool operator<(const EventWrapper& rhs) const;
			bool operator>(const EventWrapper& rhs) const;
		};

		std::priority_queue<EventWrapper> events;

	public:
		EventScheduler();
		
		void addEvent(const Event& event, unsigned int id, double weight = 1.0);
};

#endif // EVENT_SCHEDULER_H

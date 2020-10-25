
#include "EventScheduler.h" 

EventScheduler::EventScheduler() : events() {}

bool EventScheduler::EventWrapper::operator<(const EventWrapper& rhs) const {
	return this -> weight < rhs.weight;
}

bool EventScheduler::EventWrapper::operator>(const EventWrapper& rhs) const {
	return this -> weight > rhs.weight;
}

void EventScheduler::addEvent(const Event& event, unsigned int id,
	double weight) {

	this -> events.push({event, id, weight});
}

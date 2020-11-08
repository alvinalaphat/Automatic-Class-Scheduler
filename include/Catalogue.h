#ifndef CATALOGUE_H
#define CATALOGUE_H

#include "Event.h"
#include "Interval.h"
#include "json.h"
#include <string>
#include <unordered_map>

class Entry {
  int mId;
  std::string mName;
  Event mEvent;

public:
  Entry();
  Entry(int, std::string, Event);
  inline int id() const { return mId; }
  inline std::string name() const { return mName; }
  inline Event event() const { return mEvent; }
	friend std::ostream& operator<<(std::ostream&, const Entry&);
};

/*
Maybe later we can friend these functions inside the other classes.
Right now they can't access their private variables so they can't fill 'em up.

void from_json(const nlohmann::json &j, IntervalGroup &ig) {
  std::cout << "Converting " << j << " into IntervalGroup." << std::endl;
  j.get_to(ig.intervals);
}

void from_json(const nlohmann::json &j, Event &e) {
  std::cout << "Converting " << j << " into Event." << std::endl;
  j.get_to(e.sections);
}

void from_json(const nlohmann::json &j, Entry &e) {
  std::cout << "Converting " << j << " into Entry." << std::endl;
  j.at("id").get_to(e.id);
  j.at("name").get_to(e.name);
  j.at("times").get_to(e.event);
}
*/

class Catalogue {

  std::unordered_map<int, Entry> entries;

public:
  Catalogue();
	Catalogue(std::string);

  int load(std::string);
	size_t size() const;
	std::vector<int> ids() const;
	Entry get(int);
	Entry operator[](int);
	friend std::ostream& operator<<(std::ostream&, const Catalogue&);
};

#endif /* CATALOGUE_H */
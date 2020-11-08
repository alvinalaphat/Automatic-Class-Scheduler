#include <Catalogue.h>
#include <Event.h>
#include <Interval.h>
#include <fstream>
#include <iostream>
#include <json.h>
#include <string>
#include <utility>
#include <vector>


/* Will refactor Entry into another class. */
Entry::Entry() : mId(0), mName(""), mEvent() {}

Entry::Entry(int pId, std::string pName, Event pEvent)
    : mId(pId), mName(pName), mEvent(pEvent) {}

std::ostream& operator<<(std::ostream& os, const Entry& e) {
	os << "Entry with id " << e.mId << std::endl;
	os << "  Name is \"" << e.mName << "\"" << std::endl;
	e.mEvent.display(std::cout, "  ");
	return os;
}

/* Catalogue impl. */
Catalogue::Catalogue() : entries() {}
Catalogue::Catalogue(std::string filename) : entries() {
	if (not load(filename)) {
		std::cerr << "Could not load " << filename << std::endl;
	}
}

int Catalogue::load(std::string filename) {

	// read file into json if possible
  std::ifstream file(filename);
	if (not file.is_open()) {
		return EXIT_FAILURE;
	}
  nlohmann::json js;
  file >> js;

	// loop through each entry
  for (size_t i = 0; i < js.size(); ++i) {

		// prepare to build entry by preparing the parameters needed to construct it
    int id = js.at(i).at("id").get<int>();
    std::string name = js.at(i).at("name").get<std::string>();
		std::vector<IntervalGroup> secs;

		// parse interval groups to put into secs
    for (size_t k = 0; k < js.at(i).at("times").size(); ++k) {
			
      std::vector<std::pair<double, double>> intervals;
      
			// for each interval group, grab its times during the week
			for (size_t a = 0; a < js.at(i).at("times").at(k).size(); ++a) {
        intervals.push_back(
            js.at(i).at("times").at(k).at(a).get<std::pair<double, double>>());
      }

      secs.push_back(intervals);

    }

		// finally, construct it and add it to the entries map
    entries[id] = {id, name, secs};
  }

#if 0 /* Catalogue::load debug. */

	std::cout << "----------" << std::endl;
  std::cout << "Catalogue read " << entries.size() << " entries." << std::endl;
	for (auto const& [id, entry] : entries) {
		std::cout << entry;
	}
	std::cout << "----------" << std::endl;

#endif /* Catalogue::load debug. */

      return EXIT_SUCCESS;
}

size_t Catalogue::size() const {
	return entries.size();
}

std::vector<int> Catalogue::ids() const {
	std::vector<int> ret;
	for (auto const& [id, entry] : entries) {
		ret.push_back(id);
	}
	return ret;
}

Entry Catalogue::get(int id) {
	if (entries.find(id) == entries.end()) {
		return {};
	} else {
		return entries[id];
	}
}

Entry Catalogue::operator[](int id) {
	return get(id);
}

std::ostream& operator<<(std::ostream& os, const Catalogue& cat) {
	os << "Catalogue contains " << cat.size() << " entries." << std::endl;
	os << "Ids are:" << std::endl;
	for (auto const& id : cat.ids()) {
		os << "  " << id << std::endl;
	}
	os << "Entries are:" << std::endl;
	for (auto const& [id, entry] : cat.entries) {
		os << entry;
	}
	return os;
}
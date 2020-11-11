#include <Catalogue.h>
#include <Event.h>
#include <Interval.h>
#include <fstream>
#include <iostream>
#include <json.h>
#include <string>
#include <utility>
#include <vector>
#include <TopElemsHeap.h>
#include <Comparable.h>
#include <algorithm>

/* ---------------------------------------------------------------------- */

Entry::Entry() : m_id(0), m_name(""), m_event() {}

Entry::Entry(int p_id, std::string p_name, Event p_event)
    : m_id(p_id), m_name(p_name), m_event(p_event) {}

std::ostream& operator<<(std::ostream& os, const Entry& e) {
	os << "Entry with id " << e.m_id << std::endl;
	os << "  Name is \"" << e.m_name << "\"" << std::endl;
	e.m_event.display(std::cout, "  ");
	return os;
}

/* ---------------------------------------------------------------------- */

Catalogue::Catalogue() : m_entries() {}

Catalogue::Catalogue(std::string json_filename) : m_entries() {
	if (load(json_filename) == EXIT_FAILURE) {
		std::cerr << "Failure loading " << json_filename << "as catalogue!" << std::endl;
	}
}


int Catalogue::load(std::string json_filename) {

	// read file into json if possible
  std::ifstream file(json_filename);
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
    m_entries[id] = {id, name, secs};
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


std::vector<int> Catalogue::ids() const {
	std::vector<int> ret;
	for (auto const& [id, entry] : m_entries) {
		ret.push_back(id);
	}
	return ret;
}


Entry Catalogue::get(int p_id) {
	if (m_entries.find(p_id) == m_entries.end()) {
		return {};
	} else {
		return m_entries[p_id];
	}
}

Entry Catalogue::operator[](int p_id) {
	return get(p_id);
}


std::ostream& operator<<(std::ostream& os, const Catalogue& cat) {
	os << "Catalogue contains " << cat.size() << " entries." << std::endl;
	os << "Ids are:" << std::endl;
	for (auto const& id : cat.ids()) {
		os << "  " << id << std::endl;
	}
	os << "Entries are:" << std::endl;
	for (auto const& [id, entry] : cat.m_entries) {
		os << entry;
	}
	return os;
}


std::vector<Comparable<Entry>> Catalogue::search(const std::string& name, size_t max_results) const {
	TopElemsHeap<Comparable<Entry>> heap(max_results);
	for (const auto& [id, entry] : m_entries) {
		heap.push({composite_similarity(name, entry.name()), entry});
	}
	std::vector<Comparable<Entry>> result = heap.getElements();
	std::sort(result.begin(), result.end());
	return heap.getElements();
}
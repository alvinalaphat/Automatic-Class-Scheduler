#ifndef CATALOGUE_HPP
#define CATALOGUE_HPP

#include "json.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#define TIMES_TYPE std::vector<std::pair<double, double>>
#define ID_TYPE unsigned int

class Catalogue {
  std::unordered_map<ID_TYPE, TIMES_TYPE> events_;

public:
  Catalogue() : events_() {}

  void fromJson(std::string filename) {
    std::ifstream file(filename);
    nlohmann::json j;
    file >> j;

    // TODO: Here we'd validate the json with a schema.

    for (size_t i = 0; i < j.size(); ++i) {
      // Add each event to the map.
      auto id = j[i]["id"].get<ID_TYPE>();
      auto times = j[i]["times"].get<TIMES_TYPE>();
      events_[id] = times;
    }

#define CATALOGUE_VERBOSE 0
#if CATALOGUE_VERBOSE
    for (const auto &[id, times] : events_) {
      std::cout << "event #" << id << ":" << std::endl;
      for (const auto &time : times) {
        std::cout << time.first << " " << time.second << std::endl;
      }
    }
#endif /* CATALOGUE_DEBUG */
  }

  std::vector<ID_TYPE> getIds() {
    std::vector<ID_TYPE> ret;
    for (const std::pair<ID_TYPE, TIMES_TYPE> &p : events_) {
      ret.push_back(p.first);
    }
    return ret;
  }

  size_t size() { return events_.size(); }
  
  TIMES_TYPE get(ID_TYPE eventId) {
    if (events_.find(eventId) != events_.end()) {
      return events_[eventId];
    } else {
      return {};
    }
  }

  TIMES_TYPE operator[](ID_TYPE eventId) { return get(eventId); }
};

#endif /* CATALOGUE_HPP */

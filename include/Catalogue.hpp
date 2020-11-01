#ifndef CATALOGUE_HPP
#define CATALOGUE_HPP

#include "json.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using nlohmann::json;

struct Event {
    int id;
    std::vector<std::pair<double, double>> times;

	Event() : id(0), times() {}
};

class Catalogue {

    std::vector<Event> events_;

public:
    Catalogue()
        : events_()
    {
    }

    void fromJson(std::string filename)
    {
        /* TODO: JSONVerifier, verify JSON is correct format. */
        std::ifstream file(filename);
        if (not file.is_open())
            return;

        json j;
        file >> j;

		for (size_t i = 0; i < j.size(); ++i) {
			Event event;
			event.id = j[i]["id"].get<int>();
			event.times = j[i]["times"].get<std::vector<std::pair<double, double>>>();
			events_.push_back(event);
		}

		for (Event& event : events_) {
			std::cout << event.id << std::endl;
			for (std::pair time : event.times) {
				std::cout << time.first << " " << time.second << std::endl;
			}
		}
    }
};

#endif /* CATALOGUE_HPP */

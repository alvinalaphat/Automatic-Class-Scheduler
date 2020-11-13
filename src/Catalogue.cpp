#include <SearchEngine.h>
#include <Catalogue.h>
#include <Interval.h>
#include <Event.h>
#include <fstream>
#include <iostream>
#include <json.h>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

/**
 *  @brief Default constructor.
 */
Catalogue::Entry::Entry()
    : id(), name(""), event(), tags() {}

/**
 *  @brief Constructor, sets Entry's values.
 */
Catalogue::Entry::Entry(
    size_t p_id,
    std::string p_name,
    Event p_event,
    std::unordered_map<std::string, std::vector<std::string>> p_tags)
    : id(p_id), name(p_name), event(p_event), tags(p_tags) {}

/**
 *  @brief Overload Entry print. Prints each element.
 */
std::ostream& operator<<(std::ostream& os, const Catalogue::Entry& entry) {
    os <<
    "Entry with id '" << entry.id << "'\n"
    "  Name is '" << entry.name << "'\n"
    "  Has " << entry.event.size() << " section(s)\n"
    "  Tags are: ";

    // print out tags in [ tag1, tag2, tag3, ] form
    os << "[ ";
    for (const auto& [tag, _] : entry.tags) {
        os << tag << ", ";
    }
    os << ']' << std::endl;

    return os;
}

/* ---------------------------------------------------------------------- */

/**
 *  @brief We require this so we can go j.get<Event>()
 */
void from_json(const nlohmann::json& j, Event& e)
{
    std::vector<IntervalGroup> secs;
    for (size_t i = 0; i < j.size(); ++i) {
        std::vector<std::pair<double, double>> intervals;
        // for each interval group, grab its times during the week
        for (size_t a = 0; a < j.at(i).size(); ++a) {
            intervals.push_back(
                j.at(i).at(a).get<std::pair<double, double>>());
        }
        secs.push_back(intervals);
    }
    e = Event(secs);
}

/**
 *  @brief We require this so we can go j.get<Entry>()
 */
void from_json(const nlohmann::json& j, Catalogue::Entry& e)
{
    Event event = j.at("times").get<Event>();
    e = Catalogue::Entry(
        j.at("id").get<size_t>(),
        j.at("name").get<std::string>(),
        event,
        j.at("tags").get<std::unordered_map<
        std::string, std::vector<std::string>>>()
    );
}



/* ---------------------------------------------------------------------- */

Catalogue::Catalogue()
	: entries(), engine() {}

/**
 *  @brief Constructor to pre-load json courses. 
 */
Catalogue::Catalogue(std::string json_filename) : entries(), engine() {
    if (load(json_filename) == EXIT_FAILURE) {
        std::cerr << "Failure loading " 
                  << json_filename << " as catalogue!" << std::endl;
    }
}

/**
 *  @brief Loads given catalogue json. 
 *         EXIT_FAILURE on failure, EXIT_SUCCESS on success
 */
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
        // construct Entry and add it to the catalogue
        Entry ent = js.at(i).get<Catalogue::Entry>();
        engine.insert(ent.name, ent.id);
        entries[ent.id] = ent;
    }

    // Index search engine, so we can search.
    engine.index();

    return EXIT_SUCCESS;
}

/**
 *  @brief Returns the ids of this catalogue. TODO: a const_iterator that
 *         iterates over keys of unordered_map to save memory.
 */
std::vector<size_t> Catalogue::ids() const {
    std::vector<size_t> ret;
    for (auto const& [id, entry] : entries) {
        ret.push_back(id);
    }
    return ret;
}

/**
 *  @brief Returns exception if id is not found.
 */
const Catalogue::Entry& Catalogue::at(size_t p_id) {
    if (entries.find(p_id) == entries.end()) {
        throw std::out_of_range("key not found");
    } else {
        return entries.at(p_id);
    }
}

/**
 *  @brief Overload catalogue print. Prints each entry.
 */
std::ostream& operator<<(std::ostream& os, const Catalogue& cat) {
    os << "Catalogue contains " << cat.size() << " entries." << std::endl;
    for (const auto& [_, entry] : cat.entries) {
        os << entry;
    }
    return os;
}

// Returns a vector of entries that are similar to name of class.
std::vector<Catalogue::Entry> Catalogue::search(
    const std::string& name,
    size_t max_results,
    double threshold
) {
    std::vector<Entry> ret;
    for (auto const& result : engine.search(name, threshold, max_results)) {
        ret.push_back(at(result.data));
    }
    return ret;
}

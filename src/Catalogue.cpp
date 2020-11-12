#include <Catalogue.h>
#include <Interval.h>
#include <Event.h>
#include <fstream>
#include <iostream>
#include <json.h>
#include <string>
#include <vector>

/* ---------------------------------------------------------------------- */

Entry::Entry()
    : m_id(0), m_name(""), m_event(), m_tags() {}

Entry::Entry(
    size_t p_id,
    std::string p_name,
    Event p_event,
    std::unordered_map<std::string, std::vector<std::string>> p_tags)
    : m_id(p_id), m_name(p_name), m_event(p_event), m_tags(p_tags) {}

/**
 *  @brief Overload Entry print. Prints each element.
 */
std::ostream& operator<<(std::ostream& os, const Entry& entry) {
    os <<
    "Entry with id '" << entry.m_id << "'\n"
    "  Name is '" << entry.m_name << "'\n"
    "  Has " << entry.m_event.size() << " section(s)\n"
    "  Tags are: ";

    // print out tags in [ tag1, tag2, tag3, ] form
    os << "[ ";
    for (const auto& [tag, _] : entry.m_tags) {
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
void from_json(const nlohmann::json& j, Entry& e)
{
    Event event = j.at("times").get<Event>();
    e = Entry(
        j.at("id").get<size_t>(),
        j.at("name").get<std::string>(),
        event,
        j.at("tags").get<std::unordered_map<
        std::string, std::vector<std::string>>>()
    );
}

/* ---------------------------------------------------------------------- */

Catalogue::Catalogue() : m_entries() {}

/**
 *  @brief Constructor to pre-load json courses. 
 */
Catalogue::Catalogue(std::string json_filename) : m_entries() {
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
        Entry ent = js.at(i).get<Entry>();
        m_entries[ent.id()] = ent;
    }

    return EXIT_SUCCESS;
}

/**
 *  @brief Returns the ids of this catalogue. TODO: a const_iterator that
 *         iterates over keys of unordered_map to save memory.
 */
std::vector<size_t> Catalogue::ids() const {
    std::vector<size_t> ret;
    for (auto const& [id, entry] : m_entries) {
        ret.push_back(id);
    }
    return ret;
}

/**
 *  @brief Returns exception if id is not found.
 */
const Entry& Catalogue::at(size_t p_id) {
    if (m_entries.find(p_id) == m_entries.end()) {
        std::cout << p_id << std::endl;
        throw std::out_of_range("key not found");
    } else {
        return m_entries.at(p_id);
    }
}

/**
 *  @brief Overload catalogue print. Prints each entry.
 */
std::ostream& operator<<(std::ostream& os, const Catalogue& cat) {
    os << "Catalogue contains " << cat.size() << " entries." << std::endl;
    for (const auto& [_1, entry] : cat.m_entries) {
        os << entry;
    }
    return os;
}

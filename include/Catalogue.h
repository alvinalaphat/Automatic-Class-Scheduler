#ifndef CATALOGUE_H
#define CATALOGUE_H

/* ---------------------------------------------------------------------- */

#include "Event.h"
#include "Interval.h"
#include "json.h"
#include "SearchEngine.h"
#include <string>
#include <unordered_map>

/* ---------------------------------------------------------------------- */

class Catalogue
{

public:

    /**
     *  @struct To hold Catalogue entries, containing Scheduler event input, with
     *          some extra metadata.
     */
    struct Entry
    {
        size_t id;
        std::string name;
        Event event;
        std::unordered_map<std::string, std::vector<std::string>> tags;

        Entry(); // needs to be default-constructible for json.
        Entry(
            size_t p_id,
            std::string p_name,
            Event p_event,
            std::unordered_map<std::string, std::vector<std::string>> p_tags);
        
        friend std::ostream& operator<<(std::ostream&, const Entry&);
    };

    Catalogue();

    /**
     *  @brief Creates a catalogue from a given json filename.
     */
    Catalogue(std::string json_filename);

    /**
     *  @brief Loads a json file into this catalogue.
     *  @param json_filename The name of the json file to load.
     *  @return EXIT_SUCCESS on success, EXIT_FAILURE on failure. Note, on failure,
     *          this catalogue is not modified. On success, this catalogue is modified.
     */
    int load(std::string json_filename);

    /**
     *  @brief Returns the number of entries in this catalogue.
     *  @return The number of entries in this catalogue.
     */
    inline size_t size() const { return entries.size(); }

    /**
     *  @brief Returns the ids of the entries in this catalogue.
     *  @return A vector containing the ids of the entries in this catalogue.
     */
    std::vector<size_t> ids() const;

    // Both attempt to get Entry with given id, return empty Entry on failure.
    const Entry& at(size_t p_id);

    // Returns TRUE if id exists in entries, else FALSE.
    inline bool has(size_t p_id) const { return entries.find(p_id) != entries.end(); }

    friend std::ostream& operator<<(std::ostream&, const Catalogue&);

    // Returns a vector of entries that are similar to name of class.
    std::vector<Entry> search(
        const std::string& name,
        size_t max_results = 50,
        double threshold = 0.01);

private:

    std::unordered_map<size_t, Entry> entries;
    SearchEngine<size_t> engine;

};

/* ---------------------------------------------------------------------- */

#endif /* CATALOGUE_H */

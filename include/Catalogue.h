#ifndef CATALOGUE_H
#define CATALOGUE_H

/* ---------------------------------------------------------------------- */

#include "Event.h"
#include "Interval.h"
#include "json.h"
#include <string>
#include <unordered_map>

/* ---------------------------------------------------------------------- */

class Entry
{

public:

  Entry();
  Entry(
    size_t p_id,
    std::string p_name,
    Event p_event,
    std::unordered_map<std::string, std::vector<std::string>> p_tags);

  inline size_t id() const { return m_id; }
  inline const std::string& name() const { return m_name; }
  inline const Event& event() const { return m_event; }
  inline const std::unordered_map<
    std::string, std::vector<std::string>>&tags() const { return m_tags; }

  friend std::ostream& operator<<(std::ostream&, const Entry&);

private:

  size_t m_id;
  std::string m_name;
  Event m_event;
  std::unordered_map<std::string, std::vector<std::string>> m_tags;

};

/* ---------------------------------------------------------------------- */

class Catalogue
{

public:

  Catalogue();

  // Attempts to fill self (m_entries) with courses from json catalogue.
  Catalogue(std::string json_filename);

  // Fills self (m_entries) with courses from json catalogue.
  // Returns EXIT_SUCCESS on succes and EXIT_FAILURE on failure.
  int load(std::string json_filename);

  // Returns number of entries in catalogue.
  inline size_t size() const { return m_entries.size(); }

  // Returns all ids in catalogue.
  std::vector<size_t> ids() const;

  // Both attempt to get Entry with given id, return empty Entry on failure.
  const Entry& at(size_t p_id);

  // Returns TRUE if id exists in entries, else FALSE.
  inline bool has(size_t p_id) const { return m_entries.find(p_id) != m_entries.end(); }

  friend std::ostream& operator<<(std::ostream&, const Catalogue&);

private:

  std::unordered_map<size_t, Entry> m_entries;

};

/* ---------------------------------------------------------------------- */

#endif /* CATALOGUE_H */

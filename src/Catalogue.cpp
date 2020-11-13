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
#include <cmath>

/* ---------------------------------------------------------------------- */



Entry::Entry() 
	: m_id(0), m_name(""), m_event() {}

Entry::Entry(int p_id, std::string p_name, Event p_event)
	: m_id(p_id), m_name(p_name), m_event(p_event) {}

std::ostream& operator<<(std::ostream& os, const Entry& e)
{
	os << "Entry with id " << e.m_id << std::endl;
	os << "  Name is \"" << e.m_name << "\"" << std::endl;
	e.m_event.display(std::cout, "  ");
	return os;
}



/* ---------------------------------------------------------------------- */



Catalogue::Catalogue()
	: m_entries(), m_indiv_cache(), m_compos_cache() {}



Catalogue::Catalogue(std::string json_filename)
	: m_entries(), m_indiv_cache(), m_compos_cache()
{
	if (load(json_filename) == EXIT_FAILURE) {
		std::cerr << "Failure loading " << json_filename
		          << "as catalogue!" << std::endl;
	}
}



void Catalogue::cache(int id, const std::string& name)
{
	// lower case string
	std::string nname = preprocess_string(name);

	// create frequency of ngrams of name
	std::unordered_map<std::string, size_t>
		indiv_ngrams = make_ngram_freq(nname);

	// add to individual cache
	m_indiv_cache[id] = indiv_ngrams;

	// add to global cache
	combine_occ_maps(m_compos_cache, indiv_ngrams);
}



void Catalogue::add_entry(Entry ent)
{
	// cache entry and add it to internal list
	cache(ent.id(), ent.name());
	m_entries[ent.id()] = ent;
}



double Catalogue::tfidf(
	const std::unordered_map<std::string, size_t>& indiv,
	const std::unordered_map<std::string, size_t>& other
) {
	double res = 0;

	// for each ngram in the string being matched...
	for (const auto& [ngram, _] : indiv) {

		// comput term frequency
		double tf = (other.find(ngram) != other.end())
			? (double)other.find(ngram)->second : 0;

		// compute inverse document frequency
		double idf = std::log(
			(double)m_entries.size()
			/ (double)(1 + m_compos_cache[ngram]));

		// add to result
		res += tf * idf;
	}

	return res;
}



int Catalogue::load(std::string json_filename)
{
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
		cache(id, name); // so we can search later
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
		add_entry({id, name, secs});
	}

	return EXIT_SUCCESS;
}

std::vector<int> Catalogue::ids() const
{
	// basically python's dict().keys()
	std::vector<int> ret;
	for (auto const& [id, entry] : m_entries) {
		ret.push_back(id);
	}
	return ret;
}



Entry Catalogue::get(int p_id)
{
	// returns default entry if not found
	if (m_entries.find(p_id) == m_entries.end()) {
		return {};
	} else {
		return m_entries[p_id];
	}
}

std::ostream& operator<<(std::ostream& os, const Catalogue& cat)
{
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
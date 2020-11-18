#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include <TopElemsHeap.h>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <chrono>
#include <string>
#include <vector>
#include <cmath>

/**
 *  @class A simple class that prints to standard out when it is destroyed.
 */
struct Timer
{
    std::chrono::high_resolution_clock::time_point start;
    std::string tag;

    Timer(const std::string& p_tag) : start(), tag(p_tag)
    {
        start = std::chrono::high_resolution_clock::now();
    }

    virtual ~Timer()
    {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "" << tag << " took about " << duration.count() << " µs." << std::endl;
    }
};

template<typename DataType>
class SearchEngine
{
    /* ---------------------------------------------------------------- */
public:

    /**
     *  @struct SearchEntry
     *  @brief To hold internal information and also returned upon search.
     */
    struct SearchEntry
    {
        std::string name;
        DataType data;

        SearchEntry()
            : name(""), data() {}
        SearchEntry(const std::string& p_name,
            const DataType& p_data)
            : name(p_name), data(p_data) {}
    };

    /**
     *  @struct IndexedEntry
     *  @brief To hold internal information about index.
     */
    struct IndexedEntry : public SearchEntry
    {
        std::unordered_map<std::string, double> tf;
        std::unordered_map<std::string, double> tfidf;

        IndexedEntry()
            : SearchEntry(), tf(), tfidf() {}
        IndexedEntry(
            const std::string& p_name,
            const DataType& p_data)
            : SearchEntry(p_name, p_data), tf(), tfidf()
        {
            // We want to compare by lower-case, so let's do that.
            std::string l_name = SearchEntry::name;
            std::transform(l_name.begin(), l_name.end(), l_name.begin(),
                [](unsigned char c) {return std::tolower(c);});

            // Update frequency of n-grams.
            tf = SearchEngine::make_tf(l_name);
        }
    };

    /**
     *  @struct ComparableEntry
     *  @brief To sort results by similarity and not by data.
     */
    struct ComparableEntry : public SearchEntry
    {
        double value;

        ComparableEntry()
            : SearchEntry(), value(0) {}
        ComparableEntry(
            const std::string& p_name,
            const DataType& p_data,
            const double& p_value)
            : SearchEntry(p_name, p_data), value(p_value) {}

        bool operator<(const ComparableEntry& other) const { return value < other.value; }
        bool operator>(const ComparableEntry& other) const { return value > other.value; }
        bool operator<=(const ComparableEntry& other) const { return value <= other.value; }
        bool operator>=(const ComparableEntry& other) const { return value >= other.value; }
        bool operator==(const ComparableEntry& other) const { return value == other.value; }
        bool operator!=(const ComparableEntry& other) const { return not (value == other.value); }
    };

    /* ---------------------------------------------------------------- */
private:

    /**
     *  @brief To hold all added documents.
     */
    std::vector<IndexedEntry> docs;

    /**
     *  @brief To hold occurrence of terms in all documents, and to
     *         hold inverse document frequency of all terms in corpus.
     */
    std::unordered_map<std::string, size_t> occ;
    std::unordered_map<std::string, double> idf;

    /**
     *  @brief Makes the normalized term frequencies for a given string.
     *  @param s The string to make the term frequencies for.
     *  @param len The length of how to n-gram the string.
     *  @return Unordered map of form {term: count, ...}
     */
    static std::unordered_map<std::string, double> make_tf(
        const std::string& s,
        const size_t& len = 3,
        bool normalize = true
    ) {
        std::unordered_map<std::string, double> ret;
        size_t total = 0;

        // Count frequency of each word.
        for (const std::string& ngram : make_ngrams(s, len)) {
            ++ret[ngram];
            ++total;
        }

        // This can occur if query string is empty.
        if (total == 0) {
            return ret;
        }

        // Now, we will normalize the frequencies.
        if (normalize) {
            double d_total = (double)total;
            for (const auto& [ngram, _] : ret) {
                ret[ngram] /= d_total;
            }
        }

        return ret;
    }

    /**
     *  @brief Make n-grams out of a given string.
     *  @param s The given string.
     *  @param len The length of the n-grams.
     *  @return The n-grams of @p s .
     */
    static std::vector<std::string> make_ngrams(
        const std::string& s,
        const size_t& len = 3
    ) {
        std::vector<std::string> ret;

        // Pad with " " on either side, then make chunks.
        std::string pad = " " + s + " ";
        for (size_t i = 0; i + len <= pad.size(); ++i) {
            ret.push_back(pad.substr(i, len));
        }

        return ret;
    }

    /**
     *  @brief Calculate the magnitude of the values of an unordered_map.
     *  @param map The unordered_map.
     *  @return A double representing the magnitude of the @p map .
     */
    template<typename T>
    static double map_magnitude(const std::unordered_map<T, double>& map)
    {
        double res = 0;

        // Square each value.
        for (const auto& [_, b] : map) {
            res += (double)(b * b);
        }

        // Square to get magnitude.
        res = std::sqrt(res);

        return res;
    }

    /**
     *  @brief Calculate cosine silarity between two unordered maps.
     *  @param map1 The first unordered_map.
     *  @param map2 The second unordered_map.
     *  @return The cosine similarity between @p map1 and @p map2 .
     */
    static double cosine_similarity(
        const std::unordered_map<std::string, double>& map1,
        const std::unordered_map<std::string, double>& map2
    ) {
        double ret = 0.0;

        double mag1 = map_magnitude(map1);
        double mag2 = map_magnitude(map2);
        double mag = mag1 + mag2;

        if (mag == 0) {
            return ret;
        }

        for (const auto& [key, freq] : map1) {
            ret += freq * ((map2.find(key) != map2.end()) ? map2.at(key) : 0.0);
        }

        ret /= mag;

        return ret;
    }

    /* ---------------------------------------------------------------- */
public:

    /**
     *  @brief Default constructor. Nothing special.
     */
    SearchEngine()
        : docs(), occ(), idf() {}

    /**
     *  @brief Clears internal state, deletes all entries and indexing.
     */
    void clear()
    {
        docs.clear();
        occ.clear();
        idf.clear();
    }

    /**
     *  @brief Adds a name and its associated data to the search engine.
     *         Increments occurrence count for all documents.
     *  @param name The name of the entry.
     *  @param data The data to associate with the entry.
     */
    void insert(std::string name, DataType data)
    {
        IndexedEntry entry = { name, data };

        // Update occurrence count for all documents.
        for (const auto& [ngram, _] : entry.tf) {
            ++occ[ngram];
        }

        docs.push_back(entry);
    }

    /**
     *  @brief Searches the indexed entries for relevant entries.
     *  @param query The query string that can contain multiple terms.
     *  @return A vector of SearchEntrys containing relevant entries.
     */
    std::vector<ComparableEntry> search(
        const std::string& query,
        double threshold = 0.01,
        size_t max_results = 50
    ) {
        // Start timer.
        std::string tag = "Search for '" + query + "'";
        Timer timer(tag);

        // Switching to a heap of 10 elems reduces 60+ ms to ~10 ms with O0.
        TopElemsHeap<ComparableEntry> heap(max_results);

        // Make query lowercase.
        std::string l_query = query;
        std::transform(l_query.begin(), l_query.end(), l_query.begin(),
            [](unsigned char c) { return std::tolower(c); });

        // Calculate tfidf of terms in query string.
        double d_N = (double)docs.size();
        std::unordered_map<std::string, double> q_tfidf;
        for (const auto& [ngram, freq] : make_tf(l_query)) {
            if (occ.find(ngram) != occ.end()) {
                q_tfidf[ngram] = freq * (1 + std::log(d_N / (double)occ.at(ngram)));
            }
        }

        // Add indexed entries whose cosine similarity is above the threshold.
        double sim;
        for (const auto& entry : docs) {
            sim = cosine_similarity(q_tfidf, entry.tfidf);
            if (sim > threshold) {
                ComparableEntry newEntry = { entry.name, entry.data, sim };
                heap.push(newEntry);
            }
        }

        // Return as a sorted vector.
        auto ret = heap.getElements();
        std::sort(ret.rbegin(), ret.rend());
        return ret;
    }

    /**
     *  @brief Updates internal indexing mechanism.
     */
    void index()
    {
        double d_N = (double)docs.size();

        // Calculate idf for each term present in corpus.
        for (const auto& [ngram, freq] : occ) {
            idf[ngram] = 1 + std::log(d_N / (double)freq);
        }

        // Calculate tfidf for each term in every document in corpus.
        for (auto& index : docs) {
            for (const auto& [ngram, freq] : index.tf) {
                index.tfidf[ngram] = freq * idf[ngram];
            }
        }
    }

};

#endif /* SEARCHENGINE_H */

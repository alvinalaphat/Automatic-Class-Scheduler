#ifndef FUZZY_H
#define FUZZY_H

#include <string>
#include <vector>
#include <iterator>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <cmath>
#include <algorithm>
#include "Catalogue.h"
#include "TopElemsHeap.h"

/**
 *  @brief Find the frequency of each element in a container.
 *  @param it Start of container to count.
 *  @param end End of container to count.
 *  @return A std::unordered_map with the frequency of each element.
 */
template<typename Iter>
std::unordered_map<typename std::iterator_traits<Iter>::value_type, size_t>
make_frequency(Iter it, Iter end) {
    // note for later: this had to be in the header file to avoid ld errors, why?
    std::unordered_map<
        typename std::iterator_traits<Iter>::value_type, size_t> map;
    for (; it != end; ++it) {
        ++map[*it];
    }
    return map;
}

/**
 *  @brief Find the union of the keys in the given unordered_maps.
 *  @param map1 The first unordered_map.
 *  @param map2 The second unordered_map.
 *  @return A std::unordered_set with the union of the keys of @p map1 and @p map2 .
 */
template<typename T, typename U>
std::unordered_set<T> union_keys(
    const std::unordered_map<T, U>& map1,
    const std::unordered_map<T, U>& map2
);

/**
 *  @brief Calculate the magnitude of the values of an unordered_map.
 *  @param map The unordered_map.
 *  @return A double representing the magnitude of the @p map .
 */
template<typename T>
double map_magnitude(const std::unordered_map<T, size_t>& map);

/**
 *  @brief Calculates the cosine similarity between two strings.
 *  @param s1 The first string.
 *  @param s2 The second string.
 *  @return The cosine similarity between @p s1 and @p s2 .
 */
double cosine_similarity(const std::string& s1, const std::string& s2);

/**
 *  @brief Calculates the levenshtein distance between two strings.
 *  @param s1 The first string.
 *  @param s2 The second string.
 *  @return The levenshtein distance between @p s1 and @p s2 .
 */
size_t levenshtein_distance(const std::string& s1, const std::string& s2);

/**
 *  @brief Preprocesses string so that it compares easier to other strings.
 *  @param s The string to preprocess.
 *  @return The preprocessed form of @p s .
 */
std::string preprocess_string(const std::string& s);

/**
 *  @brief Calculates the similarity between two strings using a composition of
 *         different algorithms.
 *  @param s1 The first string.
 *  @param s2 The second string.
 *  @return The composite similarity between @p s1 and @p s2 .
 */
double composite_similarity(const std::string& p_s1, const std::string& p_s2);

/**
 *  @brief Returns a vector of ngrams for a given string.
 *  @param s The string.
 *  @param chunk_length The length of each chunk.
 *  @return A vector of ngrams for @p s each of length @p chunk_length.
 */
std::vector<std::string> get_ngrams(const std::string& s, size_t chunk_length = 3);

// Overload to create a new unordered_map who has the sum of values of the keys
// in the two parameters and return it. 
template <typename T, typename U>
std::unordered_map<T, U>
operator+(
    const std::unordered_map<T, U>& map1, 
    const std::unordered_map<T, U>& map2
) {
    std::unordered_map<T, U> res;
    
    for (const auto& [k, v] : map1) {
        res[k] += v;
    }

    for (const auto& [k, v] : map2) {
        res[k] += v;
    }

    return res;
}

/**
 *  @brief Extends an unordered_map with another one by summing values.
 *  @param dest The map to be extended/added to/summed into.
 *  @param src The map to be extended/summed from.
 *  @return N/A, dest is modified in-place.
 */
template <typename T, typename U>
void extend_map(
    std::unordered_map<T, U>& dest, 
    const std::unordered_map<T, U>& src
) {
// note: this also had to be in the header, weird!
    for (const auto& [k, v] : src) {
        dest[k] += v;
    }
}


/**
 *  @brief Calculates the frequency of the ngrams of a string.
 *  @param s The string to calculate the frequency of.
 *  @param chunk_length The length of each ngram.
 *  @return The frequency of the ngrams of @p s of length @p chunk_length.
 */
std::unordered_map<std::string, size_t>
make_ngram_freq(
    const std::string& s, 
    size_t chunk_length = 3);

// Overload ostream for printing vector.
template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {

    std::cout << "[ ";
    for (const auto& elem : vec) {
        os << elem << ", ";
    }
    std::cout << ']';

    os << std::endl;

    return os;
}

// Overload ostream for printing dictionary.
template <typename T, typename U>
std::ostream& operator<<(std::ostream& os, const std::unordered_map<T, U>& map) {

    std::cout << '{' << std::endl;
    for (const auto& [k, v] : map) {
        std::cout << k << ": " << v << ", " << std::endl;
    }
    std::cout << '}';

    os << std::endl;

    return os;
}

#endif /* FUZZY_H */

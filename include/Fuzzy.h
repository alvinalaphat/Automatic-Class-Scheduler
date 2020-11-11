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
make_frequency(Iter it, Iter end);

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

#endif /* FUZZY_H */

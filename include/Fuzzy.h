#ifndef FUZZY_H
#define FUZZY_H

#include <string>
#include <vector>
#include <iterator>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <cmath>

/**
 *  @brief Find the frequency of each element in a container.
 *  @param it Start of container to count.
 *  @param end End of container to count.
 *  @return A std::unordered_map with the frequency of each element.
 */
template<typename Iter>
std::unordered_map<typename std::iterator_traits<Iter>::value_type, size_t>
make_frequency(Iter it, Iter end) {
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
 *  @return A std::unordered_set with the union of the keys of @p map1 and @p map2.
 */
template<typename T, typename U>
std::unordered_set<T> union_keys(
    const std::unordered_map<T, U>& map1, 
    const std::unordered_map<T, U>& map2
) {
    std::unordered_set<T> unkeys;

    for (const auto& [a, b] : map1) {
        unkeys.insert(a);
    }

    for (const auto& [a, b] : map2) {
        unkeys.insert(a);
    }

    return unkeys;
}

/**
 *  @brief Calculate the magnitude of the values of an unordered_map.
 *  @param map The unordered_map.
 *  @return A double representing the magnitude of the @p map.
 */
template<typename T>
double map_magnitude(const std::unordered_map<T, size_t>& map) {

    double res = 0;

    for (const auto& [a, b] : map) {
        res += (double)(b * b);
    }

    res = std::sqrt(res);

    return res;
}

/**
 *  @brief Calculates the cosine similarity between two strings.
 *  @param s1 The first string.
 *  @param s2 The second string.
 *  @return The cosine similarity between @p s1 and @p s2.
 */
double cosine_similarity(const std::string& s1, const std::string& s2) {

    std::unordered_map<char, size_t> f1 = make_frequency(s1.begin(), s1.end());
    std::unordered_map<char, size_t> f2 = make_frequency(s2.begin(), s2.end());
    std::unordered_set<char> distinct = union_keys(f1, f2);

    double res = 0;

    for (const char& c : distinct) {
        res += (double)(f1[c] * f2[c]);
    }

    res /= map_magnitude(f1);
    res /= map_magnitude(f2);

    return res;
}


#endif /* FUZZY_H */

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
 *  @return A std::unordered_set with the union of the keys of @p map1 and @p map2 .
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
 *  @return A double representing the magnitude of the @p map .
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
 *  @return The cosine similarity between @p s1 and @p s2 .
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

/**
 *  @brief Calculates the levenshtein distance between two strings.
 *  @param s1 The first string.
 *  @param s2 The second string.
 *  @return The levenshtein distance between @p s1 and @p s2 .
 */
size_t levenshtein_distance(const std::string& s1, const std::string& s2) {

    size_t n = s1.size();
    size_t m = s2.size();

    if (n == 0) {
        return m;
    } else if (m == 0) {
        return n;
    }

    std::vector<
        std::vector<size_t>> matrix(n + 1, std::vector<size_t>(m + 1, 0));

    for (size_t i = 0; i < m + 1; ++i) {
        matrix.at(0).at(i) = i;
    }

    for (size_t i = 0; i < n + 1; ++i) {
        matrix.at(i).at(0) = i;
    }

    for (size_t i = 1; i < n + 1; ++i) {
        for (size_t j = 1; j < m + 1; ++j) {
            
            size_t cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;

            matrix.at(i).at(j) = std::min(
                matrix.at(i - 1).at(j) + 1,
                std::min(
                    matrix.at(i).at(j - 1) + 1,
                    matrix.at(i - 1).at(j - 1) + cost
                )
            );
        }
    }

#if 0 /* DEBUG */
    std::cout << std::endl << matrix;
#endif

    return matrix.at(n).at(m);
}

/**
 *  @brief Preprocesses string so that it compares easier to other strings.
 *  @param s The string to preprocess.
 *  @return The preprocessed form of @p s .
 */
std::string preprocess_string(const std::string& s) {

    std::string res = s;

    std::transform(res.begin(), res.end(), res.begin(),
        [](unsigned char c){return std::tolower(c); });
    
    return res;
}

/**
 *  @brief Calculates the similarity between two strings using a composition of
 *         different algorithms.
 *  @param s1 The first string.
 *  @param s2 The second string.
 *  @return The composite similarity between @p s1 and @p s2 .
 */
double composite_similarity(const std::string& p_s1, const std::string& p_s2) {

    std::string s1 = preprocess_string(p_s1);
    std::string s2 = preprocess_string(p_s2);

    double cosine_sim = cosine_similarity(s1, s2);
    double levenshtein_sim =
        ((double)std::max(s1.size(), s2.size())
        - (double)levenshtein_distance(s1, s2))
        / (double)std::max(s1.size(), s2.size());
    
    return cosine_sim * levenshtein_sim;
}

// An object that compares by a double and not by its data.
template <typename T>
class Comparable {
    double comp_;
    T data_;
public:
    Comparable() : comp_(0), data_() {}
    Comparable(const double& c, const T& d) : comp_(c), data_(d) {}

    inline bool operator>(const Comparable& oth) const { return comp_ > oth.comp_; }
    inline bool operator<(const Comparable& oth) const { return comp_ < oth.comp_; }
    inline bool operator>=(const Comparable& oth) const { return comp_ >= oth.comp_; }
    inline bool operator<=(const Comparable& oth) const { return comp_ <= oth.comp_; }
    inline bool operator==(const Comparable& oth) const { return comp_ == oth.comp_; }
    inline bool operator!=(const Comparable& oth) const { return not (comp_ == oth.comp_); }

    inline double value() const { return comp_; }
    inline T data() const { return data_; }
};

#endif /* FUZZY_H */

#include <string>
#include <vector>
#include <iterator>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <Catalogue.h>
#include <TopElemsHeap.h>
#include <Fuzzy.h>

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

template<typename T>
double map_magnitude(const std::unordered_map<T, size_t>& map) {

    double res = 0;

    for (const auto& [a, b] : map) {
        res += (double)(b * b);
    }

    res = std::sqrt(res);

    return res;
}

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

size_t levenshtein_distance(const std::string& s1, const std::string& s2) {

    size_t n = s1.size();
    size_t m = s2.size();

    if (n == 0) {
        return m;
    }
    else if (m == 0) {
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

std::string preprocess_string(const std::string& s) {

    std::string res = s;

    std::transform(res.begin(), res.end(), res.begin(),
        [](unsigned char c) {return std::tolower(c); });

    return res;
}

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

std::vector<std::string> get_ngrams(const std::string& s, size_t chunk_length) {

    std::vector<std::string> res;
    std::string p = " " + s + " ";

    size_t l = 0;

    while (l + chunk_length <= p.size()) {
        res.push_back(p.substr(l, chunk_length));
        ++l;
    }

    return res;
}

std::unordered_map<std::string, size_t>
make_ngram_freq(
    const std::string& s, 
    size_t chunk_length
) {
    std::vector<std::string> ngrams = get_ngrams(s, chunk_length);
    std::unordered_map<std::string, size_t> res = make_frequency(
        ngrams.begin(), ngrams.end());
    return res;
}
#include <iostream>
#include <vector>
#include <string>
#include <Fuzzy.h>
#include <iterator>

template<typename Iter>
std::unordered_map<typename Iter::value_type, size_t>
ooooom(Iter it, Iter end) {
    std::unordered_map<
        typename Iter::value_type, size_t> map;
    for (; it != end; ++it) {
        ++map[*it];
    }
    return map;
}

int main() {
    std::cout << "---------- BETTER FUZZY TEST ----------" << std::endl;

    // Test ngrams and frequency.

    std::string t8_string = "Hello, world!";
    std::vector<std::string> t8_ngrams = get_ngrams(t8_string);
    std::cout << "Test ngrams for '" << t8_string << "'" << std::endl;
    std::cout << t8_ngrams << std::endl;

    std::unordered_map<std::string, size_t> t8_freq = make_frequency(t8_ngrams.begin(), t8_ngrams.end());
    std::cout << "Frequency of those ngrams: " << t8_freq << std::endl;

    Catalogue cat("data/nd_courses.json");
    cat.search("first year", 5);

    std::cout << "-------- END BETTER FUZZY TEST --------" << std::endl;
    return 0;
}
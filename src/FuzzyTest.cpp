#include <iostream>
#include <iomanip>
#include <string>
#include <Fuzzy.h>

int main() {
    std::cout << "---------- FUZZY TEST ----------" << std::endl;
    
    std::cout << "Cosine similarity tests:" << std::endl << std::endl;

    // Cosine similarity - test one.
    std::string t0_s1 = "aabccdd";
    std::string t0_s2 = "ababcd";
    std::cout << "Cosine similarity of '" << t0_s1 << "' and '" << t0_s2 << "': ";
    std::cout << std::setprecision(8) << std::fixed
              << cosine_similarity("aabccdd", "ababcd") << std::endl;
    
    // Cosine similarity - test two.
    std::string t1_s1 = "car";
    std::string t1_s2 = "rac";
    std::cout << "Cosine similarity of '" << t1_s1 << "' and '" << t1_s2 << "': ";
    std::cout << std::setprecision(8) << std::fixed
              << cosine_similarity(t1_s1, t1_s2) << std::endl << std::endl;

    std::cout << "Levenshtein distance tests: " << std::endl << std::endl;

    // Levenshtein distance - test one.
    std::string t2_s1 = "GAMBOL";
    std::string t2_s2 = "GUMBO";
    std::cout << "Levenshtein distance between '" << t2_s1 << "' and '" << t2_s2 << "': ";
    std::cout << levenshtein_distance(t2_s1, t2_s2) << std::endl;
    
    // Levenshtein distance - test two.
    std::string t3_s1 = "";
    std::string t3_s2 = "Hello!";
    std::cout << "Levenshtein distance between '" << t3_s1 << "' and '" << t3_s2 << "': ";
    std::cout << levenshtein_distance(t3_s1, t3_s2) << std::endl;
    
    // Levenshtein distance - test two.
    std::string t4_s1 = "Hello!";
    std::string t4_s2 = "";
    std::cout << "Levenshtein distance between '" << t4_s1 << "' and '" << t4_s2 << "': ";
    std::cout << levenshtein_distance(t4_s1, t4_s2) << std::endl;
    
    std::cout << "-------- END FUZZY TEST --------" << std::endl;
    return 0;
}
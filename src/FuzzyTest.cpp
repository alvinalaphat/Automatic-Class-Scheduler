#include <iostream>
#include <iomanip>
#include <string>
#include <Fuzzy.h>

int main() {
    std::cout << "---------- FUZZY TEST ----------" << std::endl;
    
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
              << cosine_similarity(t1_s1, t1_s2) << std::endl;

    std::cout << "-------- END FUZZY TEST --------" << std::endl;
    return 0;
}
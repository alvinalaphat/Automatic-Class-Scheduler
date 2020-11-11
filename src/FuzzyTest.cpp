#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <Fuzzy.h>
#include <TopElemsHeap.h>
#include <Catalogue.h>

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
    std::cout << levenshtein_distance(t4_s1, t4_s2) << std::endl << std::endl;

    std::cout << "Composite similarity tests: " << std::endl << std::endl;

    // Composite similarity - test one.
    std::string t5_s1 = "data structures and algorithms";
    std::string t5_s2 = "DATA STRUCTURES";
    std::string t5_s3 = "dat structs and algos";
    std::cout << "Composite similarity between '" << t5_s1 << "' and '"
        << t5_s2 << "': ";
    std::cout << std::setprecision(2) << std::fixed
        << composite_similarity(t5_s1, t5_s2) * 100 << "% match"
        << std::endl;
    std::cout << "Composite similarity between '" << t5_s2 << "' and '"
        << t5_s3 << "': ";
    std::cout << std::setprecision(2) << std::fixed
        << composite_similarity(t5_s2, t5_s3) * 100 << "% match"
        << std::endl;
    std::cout << "composite_similarity between '" << t5_s1 << "' and '"
        << t5_s3 << "': ";
    std::cout << std::setprecision(2) << std::fixed
        << composite_similarity(t5_s1, t5_s3) * 100 << "% match"
        << std::endl << std::endl;

    std::cout << "Composite similarity - batch tests:" << std::endl << std::endl;

    // Composite similarity - heap test.

    std::string t6_query = "wring and rhetoic";
    std::vector<std::string> t6_options = {
        "Writing and Rhetoric",
        "Advanced Writing and Rhetoric",
        "Fundamentals of Computing",
        "Data Structures",
        "Systems Programming",
        "Introduction to Philosophy",
        "Intro to Fiction Writing",
    };

    std::cout << "Top three matches for '" << t6_query << "' in " << t6_options;
    TopElemsHeap<Comparable<std::string>> t6_heap(3);

    for (const std::string& str : t6_options) {
        t6_heap.push({ composite_similarity(t6_query, str), str });
    }

    for (const Comparable<std::string>& comp : t6_heap.getElements()) {
        std::cout << comp.data() << ' ' << std::setprecision(2)
            << comp.value() * 100 << "% match" << std::endl;
    }


    std::cout << "Catalogue - 'Did you mean?' tests." << std::endl << std::endl;

    // Search catalogue - test.

    size_t t7_max_results = 10;
    Catalogue cat("data/nd_courses.json");
    std::string t7_query = "FIRZT YR CHINSEE";
    std::vector<Comparable<Entry>> t7_results = cat.search(t7_query, t7_max_results);

    std::cout << "Top " << t7_max_results << " for '" << t7_query << "'" << std::endl;
    for (const auto& comp : t7_results) {
        std::cout << comp.data().name() << ' ' << comp.value() << "% match" << std::endl;
    }


    std::cout << "-------- END FUZZY TEST --------" << std::endl;
    return 0;
}
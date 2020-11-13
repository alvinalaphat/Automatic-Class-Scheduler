#include <SearchEngine.h>
#include <Catalogue.h>
#include <iostream>
#include <iomanip>
#include <algorithm>

int main()
{
    SearchEngine<size_t> search;

    Catalogue cat("data/nd_courses_2021.json");
    for (const auto& id : cat.ids()) {
        search.insert(cat.at(id).name, id);
    }
    search.index();

    auto results = search.search("moreau first year experience");
    std::cout << "Showing " << std::min((size_t)10, results.size()) 
              << " out of " << results.size() << " results." << std::endl;
    size_t i = 0;
    for (const auto& res : results) {
        if (i++ > 10) break; // Only show 10 results.
        std::cout << std::setw(32) << res.name
                  << std::setw(16) << res.data
                  << std::setw(16) << res.value * 100 << " similarity score"
                  << std::endl;
    }
    return EXIT_SUCCESS;
}
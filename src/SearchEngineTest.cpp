#include <SearchEngine.h>

int main()
{
    SearchEngine<size_t> search;
    search.insert("Machine learning teaches machine how to learn", 1);
    search.insert("Machine translation is my favorite subject", 2);
    search.insert("Term frequency and inverse document frequency is important", 3);
    search.index();

    for (const auto& res : search.search("Machine learning teaches machine how to learn")) {
        std::cout << res.name << std::endl;
    }
    return EXIT_SUCCESS;
}
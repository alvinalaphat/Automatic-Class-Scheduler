#ifndef APPLICATION_H
#define APPLICATION_H

/**
 *  Even though it is dangerous to have implementation in a header file,
 *  this is only meant to be used in a single class, so it is fine.
 */

#include <Catalogue.h>
#include <iostream>
#include <string>

/**
 *  I know these will quickly go out of date and that these are usually handled
 *  by your build system, but it's fun to have it anyway.
 */
#define MPP_VERSION_MAJOR 0
#define MPP_VERSION_MINOR 5
#define MPP_VERSION_PATCH 5

/**
 *  For readability and to avoid namespace pollution.
 */
class Application
{
/* ---------------------------------------------------------------- */
Catalogue cat;

/**
 *  @brief Prints welcome message to standard out.
 */
void PrintWelcomeMessage()
{
    std::cout << 
    "COOL                      YES                    PLACEHOLDER\n"
    "    SCHEDULING       THING   INDEED     CERTAINLY           TEXT\n"
    "              PROJECT               MOST\n";
}

/**
 *  @brief Gets a string from the user, quitting application if there's an error.
 *  @param prompt The prefix/prompt to use.
 *  @return User inputted string.
 */
std::string GetResponse(std::string prompt = "")
{
    std::cout << prompt; // prefix with prompt
    std::string response;
    if (not std::getline(std::cin, response)) { // eof or somthing else
        std::cout << std::endl << "Aborting application..." << std::endl;
        exit(EXIT_SUCCESS);
    } else {
        return response; // entire line
    }
}

/**
 *  @brief Converts a string into an integer.
 *  @param s The string to parse for an integer.
 *  @return An integer.
 */
int ParseInt(const std::string& s)
{
    int res = 0;
    try {
        res = std::stoi(s);
    } catch (std::invalid_argument& e) {
        std::cout << "Could not parse integer from input string." << std::endl;
    } catch (std::out_of_range& e) {
        std::cout << "Number is too large to fit in an integer!" << std::endl;
    } catch (...) {
        std::cerr << "Error parsing input." << std::endl;
    }
    return res;
}

/**
 *  @brief Handles user-driven search functionality.
 */
void DoSearch()
{
    std::string terms = GetResponse("Please enter \033[31msearch\033[0m term(s): ");

}

/**
 *  @brief Gives user first time help-hint.
 */
void PrintFirstTimeHelp()
{
    std::cout << "(Enter (h|H) to show the help menu.)" << std::endl;
}

/**
 *  @brief Prints the help info.
 */
void PrintHelp()
{
    std::cout <<
    "Available options:\n"
    "  (h|H)  show this \033[31mhelp\033[0m menu\n"
    "  (b|B)  \033[31mbuild\033[0m optimal/near-optimal schedule\n"
    "  (s|S)  \033[31msearch\033[0m for events\n"
    "  (l|L)  \033[31mlist\033[0m currently selected events\n"
    "  (q|Q)  \033[31mquit\033[0m the application\n"
    "  (DIG)  add a class by \033[31mid\033[0m to selected events";
}

/**
 *  @brief Handles user-input and returns whether or not user would like to finish.
 *  @return true if user is not done, false if the user is done.
 */
bool HandleInput()
{
    std::string response = GetResponse(">> ");

    switch (response.at(0)) { // check first character
        
        // Quit.
        case 'q':
        case 'Q':
            return false;
            break;
        
        // Search.
        case 's':
        case 'S':
            DoSearch();
            break;
        
        // Help.
        case 'h':
        case 'H':
            PrintHelp();
            break;
        
        // Build.
        case 'b':
        case 'B':
            std::cout << "Building schedule..." << std::endl;
            break;
        
        // DIG.
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            std::cout << "Adding event..." << std::endl;
            break;
        
        // Unknown command.
        default:
            PrintHelp();
            break;
    }

    return true;
}

/* ---------------------------------------------------------------- */
public:

/**
 *  Default constructor.
 */
Application()
    : cat()
{
}

/**
 *  @brief Called at main, essentially acting as main.
 */
int Run(bool verbose = true)
{
    /* LOAD CATALOGUE */
    if (verbose) std::cout << "Loading catalogue...";
    if (cat.load("data/nd_courses.json") == EXIT_FAILURE) {
        std::cout << "Error loading catalogue!" << std::endl;
        return EXIT_FAILURE;
    }
    if (verbose) std::cout << "Done!" << std::endl;

    /* PRINT WELCOME MESSAGE AND HANDLE INPUT */
    PrintWelcomeMessage();
    PrintFirstTimeHelp();
    while (HandleInput());

    return EXIT_SUCCESS;
}

/* ---------------------------------------------------------------- */

};

#endif /* APPLICATION_H */

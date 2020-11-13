#ifndef APPLICATION_H
#define APPLICATION_H

/**
 *  Even though it is dangerous to have implementation in a header file,
 *  this is only meant to be used in a single class, so it is fine.
 */

#include <EventScheduler.h>
#include <SearchEngine.h>
#include <Catalogue.h>
#include <Interval.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <utility>
#include <list>

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

    /**
     *  @brief To hold user selection about scheduler.
     */
    struct Selection
    {
        size_t id;
        double priority;
    };

    /* ---------------------------------------------------------------- */
    Catalogue cat;
    std::vector<Selection> sel;
    std::vector<IntervalGroup> excl;

    /**
     *  @brief Prints welcome message to standard out.
     */
    void PrintWelcomeMessage()
    {
        std::cout <<
            "COOL                      YES                    PLACEHOLDER\n"
            "    SCHEDULING       THING   INDEED     CERTAINLY           TEXT\n"
            "              PROJECT               MOST\n";
        std::cout << "There are \033[0;36m" << cat.size() << "\033[0m events available." << std::endl;
        std::cout << "(Enter (h|H) to show the help menu.)" << std::endl;
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
        }
        else {
            return response; // entire line
        }
    }

    /**
     *  @brief Converts a string into an integer.
     *  @param s The string to parse for an integer.
     *  @return An integer.
     */
    template<typename Type>
    Type
        ParseNumber(const std::string& s)
    {
        std::stringstream sstream(s);
        Type result;
        sstream >> result;
        return result;
    }

    /**
     *  @brief Handles user-driven search functionality.
     */
    void DoSearch()
    {
        // Get query term, and search catalogue.
        std::string terms = GetResponse("Please enter \033[0;36msearch\033[0m term(s): ");
        auto results = cat.search(terms, 10);
        std::cout << "Showing " << results.size() << " most relevant results." << std::endl;

        // Find longest name, so we can adapt to length of name and avoid magic numbers.
        int len = 0;
        for (const auto& result : results) {
            if ((int)result.name.size() > len) {
                len = (int)result.name.size();
            }
        }
        len += 2;

        // Print out results.
        for (const auto& result : results) {
            std::cout << std::left << "  "
                << std::setw((int)len) << result.name
                << result.id << std::endl;
        }
    }

    /**
     *  @brief Attempts to add event to selection.
     */
    void AddSelection(const std::string& str_id)
    {
        // Parse id of event.
        size_t id = ParseNumber<size_t>(str_id);

        // Return if not valid id.
        if (not cat.has(id)) {
            std::cout << "An event with that id does \033[0;36mnot\033[0m exist."
                << std::endl;
            return;
        }

        // Parse priority.
        std::string str_priority = GetResponse("Please enter an \033[0;36mintegral"
            "\033[0m priority (larger is more preferred): ");
        double priority = ParseNumber<double>(str_priority);

        // Add to list of selections.
        sel.push_back({ id, priority });
    }

    /**
     *  @brief Lists all selections.
     */
    void ListSelections()
    {
        // List selections.
        if (sel.size() == 0) {
            std::cout << "You have \033[0;36mno\033[0m current selections." << std::endl;
        }
        else {
            std::cout << "Listing all " << sel.size() << " selections." << std::endl;

            // Find longest name, so we can adapt to length of name and avoid magic numbers.
            int len = 0;
            for (const auto& s : sel) {
                if ((int)cat.at(s.id).name.size() > len) {
                    len = (int)cat.at(s.id).name.size();
                }
            }
            len += 2;

            // Now, print all selections out.
            for (const auto& s : sel) {
                std::cout << std::left << "  "
                    << std::setw((int)len) << cat.at(s.id).name
                    << std::setw(12) << s.id
                    << s.priority << std::endl;
            }
        }

        // List exclusions.
        if (excl.size() == 0) {
            std::cout << "You have \033[0;36mno\033[0m current time exclusions." << std::endl;
        }
        else {
            std::cout << "Listing all " << excl.size() << " exclusions." << std::endl;

            // Find longest number, so we can adapt to length of number.
            int len = (int)std::log((int)excl.size()) + 1 + 2;

            // Print all exclusions.
            for (size_t i = 0; i < excl.size(); ++i) {
                std::cout << std::left << "  "
                    << '#' << std::setw(len) << i
                    << excl.at(i) << std::endl;
            }
        }
    }

    /**
     *  @brief Build schedule and print it to console.
     */
    void DoBuild()
    {
        if (sel.size() == 0) {
            std::cout << "You have \033[0;36mno\033[0m current selections." << std::endl;
            return;
        }

        std::list<std::tuple<size_t, double, Event>> build;

        // Fill build.
        for (const auto& selection : sel) {
            const Event& event = cat.at(selection.id).event;
            // Only add event if none of its sections intersect any of the exclusions.
            for (size_t i = 0; i < event.size(); ++i) { // Loop through sections of event.
                for (size_t j = 0; j < excl.size(); ++j) { // Loop through exclusions.
                    if (excl.at(j).intersects(event.getSection(i))) {
                        break;
                    }
                }
            }
            build.push_back({selection.id, selection.priority, event});
        }

        if (build.size() == 0) {
            std::cout << "You have \033[0;36mno\033[0m selections after exclusions."
                << std::endl;
            return;
        }

        // No intersections in build!
        EventScheduler sched;
        for (const auto& [id, priority, event] : build) {
            sched.addEvent(event, (int)id, priority);
        }
        
        // Print out result!
        auto result = sched.buildOptimalSchedule();
    }

    /**
     *  @brief Ask user to exclude a time interval in which events will not
     *         be scheduled.
     */
    void ExcludeInterval()
    {
        size_t start_hour = ParseNumber<size_t>(
            GetResponse("Please enter starting time hour (military format): "));
        size_t start_min = ParseNumber<size_t>(
            GetResponse("Please enter starting time minutes: "));
        size_t end_hour = ParseNumber<size_t>(
            GetResponse("Please enter ending time hour (military format): "));
        size_t end_min = ParseNumber<size_t>(
            GetResponse("Please enter ending time minutes: "));
        std::string days = GetResponse("Please enter the days (MTWRF) on which"
            " this exclusion should be applied: ");

        double start = (double)start_hour * 60 + (double)start_min;
        double end = (double)end_hour * 60 + (double)end_min;

        std::vector<std::pair<double, double>> times;

        for (const char& day : days) {
            switch (day) {
            case 'm':
            case 'M':
                times.push_back({ start, end });
                break;
            case 't':
            case 'T':
                times.push_back({ start + 1440, end + 1440 });
                break;
            case 'w':
            case 'W':
                times.push_back({ start + 1440 * 2, end + 1440 * 2 });
                break;
            case 'r':
            case 'R':
                times.push_back({ start + 1440 * 3, end + 1440 * 3 });
                break;
            case 'f':
            case 'F':
                times.push_back({ start + 1440 * 4, end + 1440 * 4 });
                break;
            }
        }

        excl.push_back({ times });
    }

    /**
     *  @brief Prints the help info.
     */
    void PrintHelp()
    {
        std::cout <<
            "Available options:\n"
            "  (h|H)  show this \033[0;36mhelp\033[0m menu\n"
            "  (b|B)  \033[0;36mbuild\033[0m optimal/near-optimal schedule\n"
            "  (s|S)  \033[0;36msearch\033[0m for events\n"
            "  (l|L)  \033[0;36mlist\033[0m currently selected events\n"
            "  (q|Q)  \033[0;36mquit\033[0m the application\n"
            "  (e|E)  \033[0;36mexclude\033[0m a time interval\n"
            "  (1-9)  add a class by \033[0;36mid\033[0m to selected events\n";
    }

    /**
     *  @brief Handles user-input and returns whether or not user would like to finish.
     *  @return true if user is not done, false if the user is done.
     */
    bool HandleInput()
    {
        std::string response = GetResponse(">> ");

        // Continue handling input if they just press enter.
        if (response.size() == 0) {
            return true;
        }

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
            DoBuild();
            break;

            // List.
        case 'l':
        case 'L':
            ListSelections();
            break;

            // Exclude.
        case 'e':
        case 'E':
            ExcludeInterval();
            break;

            // DIG.
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            AddSelection(response);
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
        : cat(), sel(), excl()
    {
    }

    /**
     *  @brief Called at main, essentially acting as main.
     */
    int Run(bool verbose = true)
    {
        /* LOAD CATALOGUE */
        if (verbose) std::cout << "Loading catalogue...";
        if (cat.load("data/nd_courses_2021.json") == EXIT_FAILURE) {
            std::cout << "Error loading catalogue!" << std::endl;
            return EXIT_FAILURE;
        }
        if (verbose) std::cout << "Done!" << std::endl;

        /* PRINT WELCOME MESSAGE AND ENTER INPUT LOOP */
        PrintWelcomeMessage();
        while (HandleInput());

        return EXIT_SUCCESS;
    }

    /* ---------------------------------------------------------------- */

};

#endif /* APPLICATION_H */

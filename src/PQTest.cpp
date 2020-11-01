#include <Priority.hpp> // Priority<T>
#include <PriorityQueue.hpp> // PriorityQueue<T>
#include <iostream> // std::cout, std::endl;
#include <string> // std::string
#include <vector> // std::vector

using std::cout;
using std::endl;
using std::string;

// This is the main driver program for the tests for `PriorityQueue<T>`.
int main()
{
    std::vector<Priority<string>> TestOne_Priorities = {
        Priority<string> { "Cheering", 5 },
        Priority<string> { "The", 3 },
        Priority<string> { "Name", 7 },
        Priority<string> { "Wake", 1 },
        Priority<string> { "Echoes", 4 },
        Priority<string> { "Her", 6 },
        Priority<string> { "Up", 2 }
    };

    std::vector<Priority<string>> TestTwo_Priorities = {
        Priority<string> { "Impostors", 6 },
        Priority<string> { "An", 3 },
        Priority<string> { "Chungus", 1 },
        Priority<string> { "0", 5 },
        Priority<string> { "remain.", 7 },
        Priority<string> { "was", 2 },
        Priority<string> { "Impostor.", 4 }
    };

    /* TEST ONE - GIVEN, PRINT INITIAL PRIORITIES AND OUTPUT AFTER QUEUE. */
    PriorityQueue<string> TestOne_PriorityQueue;
    cout << "Test One - Initial Elements:" << endl;
    for (const Priority<string>& p : TestOne_Priorities) {
        TestOne_PriorityQueue.push(p);
        cout << p << endl;
    }
    cout << endl;

    cout << "Contents of TestOne_PriorityQueue:" << endl;
    while (not TestOne_PriorityQueue.empty()) {
        Priority<string> elem = TestOne_PriorityQueue.top();
        TestOne_PriorityQueue.pop();
        cout << elem << endl;
    }
    cout << endl
         << endl;

    /* TEST TWO - CREATED, PRINT INITTIAL PRIORITIES AND OUTPUT AFTER QUEUE */
    PriorityQueue<string> TestTwo_PriorityQueue;
    cout << "Test Two - Initial Elements:" << endl;
    for (const Priority<string>& p : TestTwo_Priorities) {
        TestTwo_PriorityQueue.push(p);
        cout << p << endl;
    }
    cout << endl;

    cout << "Contents of TestTwo_PriorityQueue:" << endl;
    while (not TestTwo_PriorityQueue.empty()) {
        Priority<string> elem = TestTwo_PriorityQueue.top();
        TestTwo_PriorityQueue.pop();
        cout << elem << endl;
    }

    return EXIT_SUCCESS;
}
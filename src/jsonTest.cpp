#include <fstream>
#include <iomanip>
#include <iostream>
#include <json.h>
#include <string>
#include <utility>
#include <vector>

using nlohmann::json;
using std::cout;
using std::endl;
using std::pair;
using std::setw;
using std::string;
using std::vector;

int main() {
  // Read json file.
  json example;
  std::ifstream example_file("data/example.json");
  example_file >> example;

  // Print file.
  cout << "-----------------------------" << endl;
  cout << "example.json - check manually" << endl;
  cout << "-----------------------------" << endl;
  cout << setw(2) << example << endl;
  cout << "-----------------------------" << endl;

  // Parse numbers.
  cout << "double parsing - check manually" << endl;
  cout << "-----------------------------" << endl;
  double value = example["object"]["value"].get<double>();
  cout << "value: " << value << " (expected: 42.99)" << endl;
  cout << "value + 1: " << (value + 1) << " (expected: 43.99)" << endl;
  cout << "-----------------------------" << endl;

  // Parse strings.
  cout << "string parsing - check manually" << endl;
  cout << "-----------------------------" << endl;
  string currency = example["object"]["currency"].get<string>();
  cout << "currency: " << currency << " (expected: USD)" << endl;
  cout << "currency + A: " << (currency + "A") << " (expected: USDA)" << endl;
  cout << "-----------------------------" << endl;

  // Parse vector of integers.
  cout << "vector parsing - check manually" << endl;
  cout << "-----------------------------" << endl;
  vector<int> vec = example["list"].get<vector<int>>();
  cout << "list: ";
  for (int& num : vec) {
    cout << num << " ";
  }
  cout << "(expected: 1 0 2)" << endl;
  cout << "-----------------------------" << endl;

  // Parse vector of pairs.
  cout << "vector of pairs parsing - check manually" << endl;
  cout << "-----------------------------" << endl;
  vector<pair<int, int>> pairs =
      example["pairs"].get <vector<pair<int, int>>>();
  cout << "pairs: ";
  for (pair<int, int>& p : pairs) {
    cout << "[" << p.first << ", " << p.second << "] ";
  }
  cout << "(expected: [1, 2] [3, 4] [5, 6])" << endl;
  cout << "-----------------------------" << endl;

  return EXIT_SUCCESS;
}
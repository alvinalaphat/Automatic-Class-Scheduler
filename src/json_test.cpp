#include <fstream>
#include <iomanip>
#include <iostream>
#include <json.hpp>
#include <string>

using nlohmann::json;
using std::cout;
using std::endl;
using std::setw;
using std::string;

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
  
  return EXIT_SUCCESS;
}
#include "dictmanager.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "filename parameter is required" << '\n';
    return 1;
  }
  std::string filename = argv[1];
  petrenko::DictionaryManager manager;
  try {
    manager.loadFromFile(filename);
  } catch (const char* e) {
    std::cerr << "Error loading file" << '\n';
    return 1;
  } catch (...) {
    std::cerr << "Unknown error loading file" << '\n';
    return 1;
  }
  std::string line;
  while (std::getline(std::cin, line)) {
    if (!line.empty()) {
      manager.processCommand(line);
    }
  }
  return 0;
}

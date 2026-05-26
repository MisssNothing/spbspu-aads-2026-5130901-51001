#include <iostream>
#include "processor.hpp"

int main() {
  petrenko::ProcessResult result = petrenko::processInput(std::cin);
  petrenko::printResults(std::cout, std::cerr, result);
  return result.overflow ? 1 : 0;
}

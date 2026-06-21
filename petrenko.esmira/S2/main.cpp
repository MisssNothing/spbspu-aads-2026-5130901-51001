#include <iostream>
#include <fstream>
#include <string>
#include "queue.hpp"
#include "stack.hpp"
#include "parser.hpp"

int runCalculator(std::istream& input, std::ostream& output) {
  petrenko::Queue<long long> results;
  petrenko::Stack<long long> reverse_results;

  std::string line;
  while (std::getline(input, line)) {
    if (line.empty()) {
      continue;
    }
    try {
      petrenko::Queue<std::string> tokens = petrenko::tokenizeLine(line);
      if (tokens.empty()) {
        continue;
      }
      petrenko::Queue<std::string> postfix = petrenko::infixToPostfix(tokens);
      long long result = petrenko::evaluatePostfix(postfix);
      results.push(result);
    } catch (...) {
      return 1;
    }
  }

  while (!results.empty()) {
    reverse_results.push(results.pop());
  }
  while (!reverse_results.empty()) {
    output << reverse_results.pop();
    if (!reverse_results.empty()) {
      output << " ";
    }
  }
  output << std::endl;
  return 0;
}

int main(int argc, char** argv) {
  std::istream* input = &std::cin;
  std::ifstream file;

  try {
    if (argc == 2) {
      file.open(argv[1]);
      if (!file.is_open()) {
        std::cerr << "Error" << '\n';
        return 1;
      }
      input = &file;
    } else if (argc > 2) {
      std::cerr << "Error" << '\n';
      return 1;
    }

    return runCalculator(*input, std::cout);

  } catch (...) {
    std::cerr << "Error" << '\n';
    return 1;
  }
}

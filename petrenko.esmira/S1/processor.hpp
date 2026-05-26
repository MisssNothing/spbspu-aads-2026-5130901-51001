#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include "list.hpp"
#include <string>

namespace petrenko {
  struct ProcessResult {
    bool overflow;
    bool sumOverflow;
    petrenko::List<std::string> titles;
    petrenko::List<petrenko::List<size_t>> numbers;
    petrenko::List<size_t> lastLine;
  };

  ProcessResult processInput(std::istream& in);
  void printResults(std::ostream& out, std::ostream& err, ProcessResult& result);
}

#endif

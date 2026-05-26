
#include "processor.hpp"
#include <limits>
#include <string>

namespace petrenko {
  ProcessResult processInput(std::istream& in) {
    ProcessResult result;
    result.overflow = false;
    result.sumOverflow = false;

    std::string line;
    while (std::getline(in, line)) {
      if (line.empty()) {
        continue;
      }
      std::string title;
      size_t count = 0;

      while (count < line.size() && line[count] == ' ') {
        ++count;
      }

      while (count < line.size() && line[count] != ' ') {
        title += line[count];
        ++count;
      }
      result.titles.insert(title, result.titles.getSize());

      while (count < line.size() && line[count] == ' ') {
        ++count;
      }

      petrenko::List<size_t> numbers;
      while (count < line.size()) {
        if (line[count] >= '0' && line[count] <= '9') {
          unsigned long long num = 0;
          bool numOverflow = false;

          while (count < line.size() && line[count] >= '0' && line[count] <= '9') {
            unsigned long long digit = line[count] - '0';
            if (num > std::numeric_limits<unsigned long long>::max() / 10) {
              numOverflow = true;
              result.overflow = true;
            } else if (num == std::numeric_limits<unsigned long long>::max() / 10) {
              if (digit > std::numeric_limits<unsigned long long>::max() % 10) {
                numOverflow = true;
                result.overflow = true;
              } else {
                num = num * 10 + digit;
              }
            } else {
              num = num * 10 + digit;
            }
            ++count;
          }
          if (!numOverflow) {
            numbers.insert(static_cast<int>(num), numbers.getSize());
          }
        } else {
          ++count;
        }
        while (count < line.size() && (line[count] == ' ' || line[count] == '\n')) {
          ++count;
        }
      }
      result.numbers.insert(numbers, result.numbers.getSize());
    }

    size_t maxi = 1;
    for (petrenko::LCIter<petrenko::List<size_t>> numbers = result.numbers.cbegin();
        numbers != result.numbers.cend(); ++numbers) {
      if ((*numbers).getSize() > maxi) {
        maxi = (*numbers).getSize();
      }
    }

    if (result.titles.getSize() == 0) {
      maxi = 0;
    }

    for (size_t counter = 0; counter < maxi; ++counter) {
      unsigned long long summa = 0;
      bool rowOverflow = false;
      for (petrenko::LCIter<petrenko::List<size_t>> numbers = result.numbers.cbegin();
           numbers != result.numbers.cend(); ++numbers) {
        if (counter < (*numbers).getSize()) {
          if (!rowOverflow) {
            if (summa > std::numeric_limits<size_t>::max() - (*numbers)[counter]) {
              rowOverflow = true;
              result.sumOverflow = true;
            } else {
              summa += (*numbers)[counter];
            }
          }
        }
      }
      result.lastLine.insert(static_cast<size_t>(summa), result.lastLine.getSize());
    }
    return result;
  }

  void printResults(std::ostream& out, std::ostream& err, ProcessResult& result) {
    if (result.titles.getSize() == 0) {
      out << "0\n";
      return;
    }

    bool first = true;
    for (petrenko::LIter<std::string> tit = result.titles.begin(); tit != result.titles.end(); ++tit) {
      if (!first) {
        out << ' ';
      }
      out << *tit;
      first = false;
    }
    out << "\n";

    size_t maxi = 1;
    for (petrenko::LCIter<petrenko::List<size_t>> numbers = result.numbers.cbegin();
         numbers != result.numbers.cend(); ++numbers) {
      if ((*numbers).getSize() > maxi) {
        maxi = (*numbers).getSize();
      }
    }

    if (result.titles.getSize() < 2) {
      maxi = 0;
    }

    for (size_t counter = 0; counter < maxi; ++counter) {
      bool firstInRow = true;
      for (petrenko::LCIter<petrenko::List<size_t>> numbers = result.numbers.cbegin();
           numbers != result.numbers.cend(); ++numbers) {
        if (counter < (*numbers).getSize()) {
          if (!firstInRow) {
            out << ' ';
          }
          out << (*numbers)[counter];
          firstInRow = false;
        }
      }
      out << "\n";
    }

    if (result.overflow || result.sumOverflow) {
      result.overflow = true;
      err << "overflow\n";
      return;
    }

    first = true;
    for (petrenko::LCIter<size_t> sums = result.lastLine.cbegin(); sums != result.lastLine.cend(); ++sums) {
      if (!first) {
        out << ' ';
      }
      out << *sums;
      first = false;
    }
    out << "\n";
  }
}

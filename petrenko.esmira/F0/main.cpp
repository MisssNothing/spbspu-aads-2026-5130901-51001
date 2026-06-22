#include "dictdispatcher.hpp"

int main()
{
  petrenko::DictDispatcher dispatcher;
  std::string line;

  while (std::getline(std::cin, line))
  {
    if (line.empty())
    {
      continue;
    }
    dispatcher.processCommand(line);
  }

  return 0;
}

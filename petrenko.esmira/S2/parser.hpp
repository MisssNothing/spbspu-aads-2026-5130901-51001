#ifndef PARSER_HPP
#define PARSER_HPP
#include <string>
#include "stack.hpp"
#include "queue.hpp"

namespace petrenko {
  int getPrecedence(const std::string& op);
  bool isOperator(const std::string& token);
  bool isDigit(char c);
  bool isNumber(const std::string& token);
  long long applyOperator(const std::string& op, long long a, long long b);
  Queue<std::string> tokenizeLine(const std::string& line);
  Queue<std::string> infixToPostfix(Queue<std::string> infix);
  long long evaluatePostfix(Queue<std::string> postfix);
}
#endif
